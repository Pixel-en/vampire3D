#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Image.h"
#include "Engine/SphereCollider.h"
#include "Engine/BoxCollider.h"
#include "Engine/CsvReader.h"
#include "Engine/SceneManager.h"
#include "Engine/TextFont.h"
#include <algorithm>

#include "Field.h"
#include "EnemySpawn.h"
#include "Knife.h"
#include "HUD.h"

namespace {
	const float MOVESPEED{ 50.0f };
	const float PLAYERHEIGHT{ 2.5f };
	const float ROTATESPEED{ 70.0f };
	const float RAYHEIGHT{ 5.0f };
	const float GRAVITY{ 9.8f / 2.0f / 60.0f };
	const float INVINCIBLETIME{ 0.5f };
	const int FONTSIZE{ 35 };
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"), hModel_(-1), hImage_(-1)
{
	transform_.position_ = { 0,2,0 };
	prePos_ = transform_.position_;
	lookHeight_ = PLAYERHEIGHT;
	onGround_ = false;
	gravity = 0.0f;
	crossTrans = transform_;
	crossTrans.position_ = { 0,0,0 };


	status_.hp_ = 10;
	status_.strength_ = 1.0f;
	status_.speed_ = 0;
	status_.critical_ = 0.0f;
	status_.criticalBoost_ = 1.2f;
	status_.collectionRange_ = 30.0f;
	status_.haste_ = 10.0f;
	status_.area_ = 1.0f;
	status_.resist_ = 0.0;
	status_.level_ = 1;
	status_.currentExp_ = 0;
	status_.nextLvExp_ = 3;
	status_.totalExp_ = 0;
}

Player::~Player()
{
}

void Player::Initialize()
{
	WeaponCSVLoad();
	PlayerStatusLoad();

	hModel_ = Model::Load("Assets\\Model\\Character\\Player.fbx");
	HandleCheck(hModel_, "プレイヤーモデルがない");

	hImage_ = Image::Load("Assets\\Image\\UI\\Test_Crosshair.png");
	HandleCheck(hImage_, "クロスヘアがない");

	//SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1.5, 0), 2);
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 1, 0), XMFLOAT3(1.0f, 3.0, 1.0f));
	AddCollider(collision);

	Knife* k = Instantiate<Knife>(GetParent());
	MyWeaponList_.push_back(k);
}

void Player::SuperUpdate()
{
}

void Player::Update()
{
	StatusUpdate();

	Move();


	if (InvincibleTimer_ > 0.0) {
		InvincibleTimer_ -= Time::DeltaTime();
		if (InvincibleTimer_ <= INVINCIBLETIME / 2.0f)
			Input::SetPadVibration(0, 0);
	}

#ifdef _DEBUG
	if (Input::IsKeyDown(DIK_Y)) {
		AcquisitionEXP(10);
	}
	if (Input::IsKeyDown(DIK_T)) {
		AcquisitionEXP(1000);
	}
#endif
}

void Player::WeaponCSVLoad()
{
	//武器のステータスを読み込む
	CsvReader csv;
	csv.Load("Assets\\CSV\\WeaponInitStatus.csv");

	for (int i = 0; i < csv.GetHeight(); i++) {
		std::string str;
		str = csv.GetString(0, i);
		WeaponObject::Status Wstate;
		Wstate.Lv_ = 1;
		Wstate.damege_ = csv.GetValue(1, i);
		Wstate.speed_ = csv.GetValue(2, i);
		Wstate.hp_ = csv.GetValue(3, i);
		Wstate.restart_ = csv.GetValue(4, i);
		Wstate.Range_ = csv.GetValue(5, i);
		Wstate.duration_ = csv.GetValue(6, i);
		Wstate.size_ = csv.GetValue(7, i);
		int knock = csv.GetValue(8, i);
		knock = std::clamp(knock, 0, 2);
		switch (knock)
		{
		case 0:
			Wstate.knockback_ = 0.0;
			break;
		case 1:
			Wstate.knockback_ = 0.5f;
			break;
		case 2:
			Wstate.knockback_ = 1.0f;
			break;
		}
		WeaponState_.insert({ str,Wstate });
	}
}

void Player::PlayerStatusLoad()
{
	//プレイヤーのステータスを読み込む
	CsvReader csv;
	csv.Load("Assets\\CSV\\PlayerStatus.csv");

	Basestatus_.hp_ = csv.GetValue(1, 2);
	Basestatus_.speed_ = csv.GetValue(2, 2);
	Basestatus_.strength_ = csv.GetValue(3, 2);
	Basestatus_.critical_ = csv.GetValue(4, 2);
	Basestatus_.collectionRange_ = csv.GetValue(5, 2);
	Basestatus_.haste_ = csv.GetValue(6, 2);
	Basestatus_.criticalBoost_ = csv.GetValue(7, 2);
	Basestatus_.area_ = csv.GetValue(8, 2);
	Basestatus_.resist_ = csv.GetValue(9, 2);
	Basestatus_.ExpBoost_ = csv.GetValue(10, 2);
	Basestatus_.maxHp_ = Basestatus_.hp_;

	Booststatus_.hp_ = 1;		//hpは使わないかも
	Booststatus_.speed_ = 1;
	Booststatus_.strength_ = 1;
	Booststatus_.critical_ = 1;
	Booststatus_.collectionRange_ = 1;
	Booststatus_.haste_ = 0.0f;
	Booststatus_.criticalBoost_ = 1;
	Booststatus_.area_ = 1;
	Booststatus_.resist_ = 1;
	Booststatus_.ExpBoost_ = 1;
	Booststatus_.maxHp_ = 0;

	status_.hp_ = Basestatus_.hp_ * Booststatus_.hp_;

	StatusUpdate();
}

bool Player::WeaponStateWrite(std::string name, WeaponObject::Status& _state)
{
	auto itr = WeaponState_.find(name);
	if (itr != WeaponState_.end()) {
		_state = itr->second;
		return true;
	}
	return false;
}

void Player::HealingHp(int _hp)
{
	if (status_.hp_ >= status_.maxHp_)
		return; //体力が最大なら何もしない
	status_.hp_ += _hp;
	if (status_.hp_ > status_.maxHp_)
		status_.hp_ = status_.maxHp_; //体力が最大値を超えないようにする

	if (status_.hp_ < 1) {
		status_.hp_ = 1; //体力が1未満にならないようにする
	}

}

void Player::StatusUpdate()
{
	//ステータスの更新
	//status_.hp_ = Basestatus_.hp_ * Booststatus_.hp_;
	status_.speed_ = Basestatus_.speed_ * Booststatus_.speed_;
	status_.strength_ = Basestatus_.strength_ * Booststatus_.strength_;
	status_.critical_ = Basestatus_.critical_ * Booststatus_.critical_;
	status_.collectionRange_ = Basestatus_.collectionRange_ * Booststatus_.collectionRange_;
	status_.haste_ = Basestatus_.haste_ + Booststatus_.haste_;		//加算
	status_.criticalBoost_ = Basestatus_.criticalBoost_ * Booststatus_.criticalBoost_;
	status_.area_ = Basestatus_.area_ * Booststatus_.area_;
	status_.resist_ = Basestatus_.resist_ * Booststatus_.resist_;
	status_.ExpBoost_ = Basestatus_.ExpBoost_ * Booststatus_.ExpBoost_;
	status_.maxHp_ = Basestatus_.maxHp_ + Booststatus_.maxHp_;		//加算
}

bool Player::isMove()
{
	return moveNow_;
}

void Player::Move()
{
	//移動方向ベクトル
	XMVECTOR moveDirVec = XMVECTOR{ 0,0,0 };
	//カメラターゲット用ベクトル
	XMVECTOR camtarVec = XMVECTOR{ 0, 0, 1, 0 };

	XMVECTOR Gravity = XMVECTOR{ 0,0,0 };
	//重力方向のベクトルを作る
	if (!onGround_) {
		gravity += GRAVITY;
		Gravity = XMVECTOR{ 0,-1,0 };
	}
	else {
		Gravity = XMVECTOR{ 0,0,0 };
		gravity = 0.0f;
	}

	Gravity = XMVector3Normalize(Gravity);

	//---------------操作--------------------
	//移動
	if (onGround_) {
		//初期化
		moveNow_ = false;

		if (Input::IsKey(DIK_W) || Input::GetPadStickL().y >= 0.5f) {
			moveDirVec = XMVectorSetZ(moveDirVec, 1.0);
			moveNow_ = true; //移動中
		}
		if (Input::IsKey(DIK_S) || Input::GetPadStickL().y <= -0.5f) {
			moveDirVec = XMVectorSetZ(moveDirVec, -1.0);
			moveNow_ = true; //移動中
		}
		if (Input::IsKey(DIK_A) || Input::GetPadStickL().x <= -0.5f) {
			moveDirVec = XMVectorSetX(moveDirVec, -1.0);
			moveNow_ = true; //移動中
		}
		if (Input::IsKey(DIK_D) || Input::GetPadStickL().x >= 0.5f) {
			moveDirVec = XMVectorSetX(moveDirVec, 1.0);
			moveNow_ = true; //移動中
		}
	}

	//カメラ縦
	if (Input::IsKey(DIK_UP) || Input::GetPadStickR().y >= 0.5f)
		lookHeight_ += 1.0f * Time::DeltaTime();
	if (Input::IsKey(DIK_DOWN) || Input::GetPadStickR().y <= -0.5f)
		lookHeight_ -= 1.0f * Time::DeltaTime();
	lookHeight_ = std::clamp(lookHeight_, PLAYERHEIGHT - 1.0f, PLAYERHEIGHT + 1.0f);


	//カメラ回転
	if (Input::IsKey(DIK_LEFT) || Input::GetPadStickR().x <= -0.5f)
		transform_.rotate_.y -= ROTATESPEED * Time::DeltaTime();
	if (Input::IsKey(DIK_RIGHT) || Input::GetPadStickR().x >= 0.5f)
		transform_.rotate_.y += ROTATESPEED * Time::DeltaTime();


	//---------------操作--------------------

	//Y軸の回転をマトリクスに変換
	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	//移動方向ベクトルに回転マトリクスをかけ回転させたベクトルを作る
	XMVECTOR rotMoveVec = XMVector3TransformCoord(moveDirVec, rot);
	rotMoveVec = XMVector3Normalize(rotMoveVec);

	//ターゲット用ベクトルに回転マトリクスをかけ回転させたベクトルを作る
	XMVECTOR rotCamtarVec = XMVector3TransformCoord(camtarVec, rot);
	rotCamtarVec = XMVector3Normalize(rotCamtarVec);


	//カメラ
	//前フレームの位置でカメラを動かすことで壁でがくがくするのを防ぐ
	if (Input::IsKey(DIK_SPACE)) {
		//俯瞰モード
		Camera::SetPosition({ transform_.position_.x,transform_.position_.y + 15,transform_.position_.z - 10 });
		Camera::SetTarget(transform_.position_);
	}
	else {
		//一人称モード
		Camera::SetPosition({ transform_.position_.x,transform_.position_.y + PLAYERHEIGHT,transform_.position_.z });
		XMFLOAT3 tarPos = transform_.position_ + 1.0f * rotCamtarVec;
		Camera::SetTarget({ tarPos.x, tarPos.y + lookHeight_, tarPos.z });
	}

	//移動前の場所を取っておく
	prePos_ = transform_.position_;
	//移動方向を取っておく
	MoveVec_ = rotMoveVec;

	//移動
	transform_.position_ += rotMoveVec * status_.speed_ * Time::DeltaTime() + Gravity * gravity;

	//フィールドからモデルのハンドルをとってくる
	Field* field = GetParent()->FindGameObject<Field>();
	NullCheck(field);
	//レイがあたった距離下げる
	onGround_ = false;
	if (field->RayCastField(transform_.position_, RAYHEIGHT, "Player")) {
		onGround_ = true;
	}

	//カメラ場所をと見ている場所をとる
	XMFLOAT3 tar = { 0,0,0 };
	XMStoreFloat3(&tar, rotCamtarVec);
	LookPos_ = { transform_.position_.x,transform_.position_.y + PLAYERHEIGHT,transform_.position_.z };
	LookTarget_ = { tar.x, tar.y + lookHeight_ - PLAYERHEIGHT, tar.z };
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	if (Input::IsKey(DIK_SPACE))
		Model::Draw(hModel_);

	Image::SetTransform(hImage_, crossTrans);
	Image::Draw(hImage_);

	FontData data;
	data.font = TextFont::GetFontName(FontList::Gkktt);
	data.Color = D2D1::ColorF(255, 255, 255);
	data.fontSize = FONTSIZE;
#ifdef _DEBUG
	TextFont::Draw("x=" + std::to_string(transform_.position_.x), { 30, 200 }, data);
	TextFont::Draw("y=" + std::to_string(transform_.position_.y), { 30, 230 }, data);
	TextFont::Draw("z=" + std::to_string(transform_.position_.z), { 30, 260 }, data);
	//TextFont::Draw("体力" + std::to_string(status_.hp_), { 30,30 }, data);
#endif
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectTag() == "Enemy")
	{
		if (InvincibleTimer_ > 0.0f)
			return;

		EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
		std::vector<Enemy*> List = ep->GetEnemyList();
		for (int i = 0; i < List.size(); i++) {
			if (dynamic_cast<Enemy*>(pTarget)->GetEnemyNumber() == List[i]->GetEnemyNumber()) {
				//ダメージを取得
				int damege = List[i]->CausedDamege();
				if (damege - status_.resist_ <= 0)
					return; //ダメージが0以下なら何もしない

				status_.hp_ -= (damege - status_.resist_);
				Input::SetPadVibration(0.5f * 65535, 0.5f * 65535);
				if (status_.hp_ <= 0) {
					SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
					sc->ChangeScene(SCENE_ID_GAMEOVER);
					Input::SetPadVibration(0, 0);
				}
				isDamege_ = true;
				InvincibleTimer_ = INVINCIBLETIME;
				break;
			}
		}
	}
}

void Player::OnCollisionsList(GameObject* pTarget, std::list<Collider*>::iterator MyItr, std::list<Collider*> list)
{
	if (pTarget->GetObjectName() == "Field") {
		//移動前の戻す
		transform_.position_ = prePos_;

		//移動方向をXとZに分解
		XMVECTOR VecX = XMVectorSet(XMVectorGetX(MoveVec_), 0, 0, 0);
		XMVECTOR VecZ = XMVectorSet(0, 0, XMVectorGetZ(MoveVec_), 0);

		transform_.position_ += VecX * status_.speed_ * Time::DeltaTime();

		bool hit = false;

		for (auto TargetItr = list.begin(); TargetItr != list.end(); ++TargetItr) {
			if ((*MyItr)->IsHit(*TargetItr)) {
				transform_.position_ = prePos_;
				hit = true;
			}
		}
		if (!hit)
			prePos_ = transform_.position_;

		hit = false;
		transform_.position_ += VecZ * status_.speed_ * Time::DeltaTime();
		for (auto TargetItr = list.begin(); TargetItr != list.end(); ++TargetItr) {
			if ((*MyItr)->IsHit(*TargetItr)) {
				transform_.position_ = prePos_;
				hit = true;
			}
		}
		if (!hit)
			prePos_ = transform_.position_;

	}
}

void Player::AcquisitionEXP(int _exp)
{
	status_.currentExp_ += (_exp * status_.ExpBoost_);

	if (status_.currentExp_ >= status_.nextLvExp_) {
		status_.level_++;							//レベルアップ
		status_.totalExp_ += status_.currentExp_;	//トータルに加算
		status_.currentExp_ -= status_.nextLvExp_;	//余剰分を算出

		HUD* hud = GetRootJob()->FindGameObject<HUD>();
		NullCheck(hud);
		//レベルアップする
		hud->LevelUP();
		EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
		ep->AddEnemyNum();	//敵の数を増やす

		//次のレベルに必要な経験値を計算
		if (status_.level_ <= 20)
			status_.nextLvExp_ += 10;
		else if (status_.level_ <= 40) {
			status_.nextLvExp_ += 13;
		}
		else {
			status_.nextLvExp_ += 16;
		}
	}

}
