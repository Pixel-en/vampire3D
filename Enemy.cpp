#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Engine/BoxCollider.h"
#include <algorithm>

#include "Field.h"
#include "Player.h"
#include "EXPManager.h"


using std::string;
namespace {
	const float GRAVITY{ 9.8f / 2.0f / 60.0f };	//重力
	const float HIGHDISTANCE{ 40.0f };	//Highモデルの距離
	const float MIDDLEDISTANCE{ 70.0f };	//Middleモデルの距離
	const float LOWDISTANCE{ 100.0f };	//Lowモデルの距離
	const int RAYHEIGHT{ 5 }; //レイの高さ
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "")
{
	objectTag_ = "Enemy";
}

Enemy::Enemy(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
	objectTag_ = "Enemy";
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	ModelHP_ = HP::FULL;
	ModelLOD_ = LOD::HIGH;
	ModelAnim_ = ANIMATION::MOVE;
	BeModelAnim_ = ANIMATION::AMAX;

	onGround_ = false;
}

void Enemy::Load(ELEVEL _level, unsigned int _number)
{
	status_.number_ = _number;
	status_.level_ = _level;

	string Level[ELEVEL::END] = { "Blue","Yellow","Green","Red" };

	//CSVからステータスの読み込み
	CsvReader csv;
	csv.Load("Assets\\CSV\\EnemyStatus.csv");
	for (int i = 1;i < csv.GetHeight();i++) {
		if (csv.GetString(0, i) == objectName_ && csv.GetString(1, i) == Level[_level]) {
			status_.power_ = csv.GetValue(2, i);
			status_.speed_ = csv.GetValue(3, i);
			status_.hp_ = csv.GetValue(4, i);
			status_.maxhp_ = status_.hp_;
			status_.exp_ = csv.GetValue(5, i);
			status_.invincibletime_ = csv.GetValue(6, i);
		}
	}

	//モデルのロード
	string hp[HP::HMAX] = { "Full", "Half", "Mini" };
	string anim[ANIMATION::AMAX] = { "Move", "Hit", "Death" };
	string lod[LOD::LMAX] = { "High", "Middle", "Low" };
	for (int i = 0;i < HP::HMAX;i++) {
		for (int j = 0;j < LOD::LMAX;j++) {
			for (int k = 0;k < ANIMATION::AMAX;i + k++) {
				hModel_[i][j][k] = -1; //初期化
				hModel_[i][j][k] = Model::Load("Assets\\Model\\Character\\Enemy\\" + objectName_ + "-" + Level[status_.level_] + "-" + hp[i] + "-" + lod[j] + "-" + anim[k] + ".fbx");
				//hModel_[i][j][k] = Model::Load("Assets\\Model\\Character\\Enemy\\EnemyOrigin\\Enemy-" + lod[j] + "-" + anim[k] + ".fbx");
				HandleCheck(hModel_[i][j][k], Level[status_.level_] + "," + hp[i] + "," + lod[j] + "," + anim[k] + "のEnemyモデルがない");

			}
		}
	}

	SetAnimation();

}

void Enemy::SuperUpdate()
{
	Model::AnimPause(hModel_[ModelHP_][ModelLOD_][ModelAnim_]);
}

void Enemy::Update()
{
	Model::AnimPlay(hModel_[ModelHP_][ModelLOD_][ModelAnim_]);

	switch (ModelAnim_)
	{
	case ANIMATION::MOVE:
		Move();
		break;
	case ANIMATION::HIT:
		if (Model::GetAnimFrame(hModel_[ModelHP_][ModelLOD_][ModelAnim_]) >= GetHitFrame()) {
			ModelAnim_ = ANIMATION::MOVE; //ヒットアニメーションが終わったら移動アニメーションに戻る
		}
		break;
	case ANIMATION::DEATH:
		if (Model::GetAnimFrame(hModel_[ModelHP_][ModelLOD_][ModelAnim_]) >= GetDeathAnimFrame()) {
			EXPManager* EManager = GetRootJob()->FindGameObject<EXPManager>();
			EManager->SpawnEXP(transform_.position_, status_.exp_);
			KillMe();
		}
		break;
	default:
		break;
	}

	//死んでいないときは無敵時間の計算をする
	if (ModelAnim_ != ANIMATION::DEATH) {
		if (InvincibleTimer_ > 0.0f) {
			InvincibleTimer_ -= Time::DeltaTime();
		}
	}
}

void Enemy::Move()
{
	Field* field = GetRootJob()->FindGameObject<Field>();
	NullCheck(field);
	Player* player = GetRootJob()->FindGameObject<Player>();
	NullCheck(player);

	//重力方向
	XMVECTOR Gravity = XMVECTOR{ 0,0,0 };

	//重力方向のベクトルを作る
	if (!onGround_) {
		gravity_ += GRAVITY;
		Gravity = XMVECTOR{ 0,-1,0 };
	}
	else {
		Gravity = XMVECTOR{ 0,0,0 };
		gravity_ = 0.0f;
	}

	XMFLOAT3 pPos = player->GetPosition();
	XMFLOAT3 ePos = transform_.position_;

	XMVECTOR pVec = XMLoadFloat3(&pPos);
	XMVECTOR eVec = XMLoadFloat3(&ePos);

	//プレイヤーと敵のベクトルを取る
	XMVECTOR epDistance = pVec - eVec;

	//LODの設定
	if (XMVectorGetX(XMVector3Length(epDistance)) <= (LOWDISTANCE * 2.5)) {
		if (XMVectorGetX(XMVector3Length(epDistance)) <= LOWDISTANCE) {
			ModelLOD_ = LOD::LOW;
			if (XMVectorGetX(XMVector3Length(epDistance)) <= MIDDLEDISTANCE) {
				ModelLOD_ = LOD::MIDDLE;
				if (XMVectorGetX(XMVector3Length(epDistance)) <= HIGHDISTANCE) {
					ModelLOD_ = LOD::HIGH;
				}
			}
		}
		else {
			onGround_ = false; //プレイヤーから遠いときは地面がないので落下しないようにする
			ModelLOD_ = LOD::LMAX;
		}
	}
	else {
		//敵がプレイヤーからあまりに遠いときは殺す
		Debug::Log("あまりにも離れすぎた");
		KillMe();
	}
	Debug::Log(XMVectorGetX(XMVector3Length(epDistance)), true);


	//回転のマトリクスを作る
	XMMATRIX RotMat = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	//マトリクスに合わせて前方向のベクトルを回転
	XMVECTOR eFrontVec = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), RotMat);

	epDistance = XMVector3Normalize(epDistance);
	eFrontVec = XMVector3Normalize(eFrontVec);

	float dot = XMVectorGetX(XMVector3Dot(eFrontVec, epDistance));	//内積を求める
	//内積がacosの範囲外に出たとき除外する
	dot = std::clamp(dot, -1.0f, 1.0f);

	float angle = acos(dot);	//角度を出す

	//外積を求めてどちらの方向に回転するか決める
	XMVECTOR cross = XMVector3Cross(epDistance, eFrontVec);
	cross = XMVector3Normalize(cross);
	if (XMVectorGetY(cross) >= 0) {
		transform_.rotate_.y += -XMConvertToDegrees(angle);
	}
	else {
		transform_.rotate_.y += XMConvertToDegrees(angle);
	}

	//移動前の場所を取っておく
	prePos_ = transform_.position_;
	transform_.position_ += epDistance * status_.speed_ * Time::DeltaTime() + Gravity * gravity_;

	if (ModelLOD_ < LOD::LMAX) {
		//重力判定するか
		onGround_ = false;
		if (field->RayCastField(transform_.position_, RAYHEIGHT)) {
			onGround_ = true;
		}
	}
	else {
		//敵が描画されてないときは重力をなくす
		onGround_ = true;
	}

	//地面下に落下したときは死ぬ
	if (transform_.position_.y <= -2000) {
		Debug::Log("落下：");
		Debug::Log(transform_.position_, true);
		KillMe();
	}
}

void Enemy::Draw()
{

	Transform tempTrans = transform_;
	tempTrans.rotate_.y += 180; //モデルが前後反転するため一時的に

	Model::SetTransform(hModel_[ModelHP_][ModelLOD_][ModelAnim_], tempTrans);
	if (ModelLOD_ != LOD::LMAX)
		Model::Draw(hModel_[ModelHP_][ModelLOD_][ModelAnim_]);
}

void Enemy::Release()
{
	for (int i = 0; i < HP::HMAX; i++) {
		for (int j = 0; j < LOD::LMAX; j++) {
			for (int k = 0; k < ANIMATION::AMAX; k++) {
				if (hModel_[i][j][k] != -1) {
					Model::Release(hModel_[i][j][k]);
					hModel_[i][j][k] = -1;
				}
			}
		}
	}
}

bool Enemy::HitDamege(int _damege, float _knock)
{
	if (InvincibleTimer_ > 0.0f)
		return false; //無敵時間中はダメージを受けない

	Player* player = GetRootJob()->FindGameObject<Player>();

	float dBoost = 1.0;
	if (player != nullptr) {
		dBoost = player->GetStatus().strength_;
		if (rand() % 100 < player->GetStatus().critical_) {
			dBoost *= player->GetStatus().criticalBoost_;
		}
	}

	status_.hp_ -= _damege * dBoost;
	InvincibleTimer_ = status_.invincibletime_;

	if (status_.hp_ <= 0) {
		ModelAnim_ = ANIMATION::DEATH;
	}
	else {
		float HPratio = (float)status_.hp_ / status_.maxhp_;
		if (HPratio >= 0.5f) {
			ModelHP_ = HP::FULL;
		}
		else if (HPratio >= 0.3f) {
			ModelHP_ = HP::HALF;
		}
		else {
			ModelHP_ = HP::MINI;
		}

		Player* player = GetRootJob()->FindGameObject<Player>();
		if (player == nullptr)
			return true;

		XMFLOAT3 pPos = player->GetPosition();
		XMFLOAT3 ePos = transform_.position_;

		XMVECTOR pPosVec = XMLoadFloat3(&pPos);
		XMVECTOR ePosVec = XMLoadFloat3(&ePos);

		//プレイヤーと敵の距離のベクトル
		XMVECTOR epDistance = pPosVec - ePosVec;

		//ノックバック
		XMVECTOR knockVec = epDistance;

		knockVec = XMVectorSetY(knockVec, 0);
		knockVec = XMVector3Normalize(knockVec);

		transform_.position_ += -knockVec * status_.speed_ * _knock;
		ModelAnim_ = ANIMATION::HIT; //ヒットアニメーションにする
	}
	return true;
}

void Enemy::OnCollisionsList(GameObject* pTarget, std::list<Collider*>::iterator MyItr, std::list<Collider*> list)
{
	if (pTarget->GetObjectName() == "Field") {
		//移動前の戻す
		transform_.position_ = prePos_;

		//プレイヤーの位置を取得
		Player* player = GetRootJob()->FindGameObject<Player>();
		XMVECTOR VecX = XMVector3Normalize(XMVectorSet(player->GetPosition().x - transform_.position_.x, 0, 0, 0));
		XMVECTOR VecZ = XMVector3Normalize(XMVectorSet(0, 0, player->GetPosition().z - transform_.position_.z, 0));

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
