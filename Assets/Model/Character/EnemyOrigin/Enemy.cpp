#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include <algorithm>
#include "Engine/Input.h"

#include "Player.h"
#include "Field.h"
#include "EXPManager.h"
#include "Engine/Camera.h"

namespace {
	const float MOVESPEED{ 5.0f };
	const float INVICIBLETIME{ 0.5f };
	const float LOWDISTANCE{ 50.0f };

	const int MOVEENDFRAME{ 22 };
	const int DEATHENDFRAME{ 35 };
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), hModelLow_(-1)
{

	status_.power_ = 5;
	status_.speed_ = 5.0f;
	status_.hp_ = 5;
	status_.exp_ = 2;


	status_.speed_ = MOVESPEED;
	transform_.position_ = { 0,0,0 };
	prePos_ = transform_.position_;

	for (int i = 0; i < ANIMATION::MAX; i++) {
		hModel_[i] = -1;
	}

	anim_ = ANIMATION::MOVE;

}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_[ANIMATION::MOVE] = Model::Load("Assets\\Model\\Character\\Enemy-Run.fbx");
	hModel_[ANIMATION::DEATH] = Model::Load("Assets\\Model\\Character\\Enemy-Death.fbx");
	for (int i = 0; i < ANIMATION::MAX; i++) {
		HandleCheck(hModel_[i]);
	}

	Model::SetAnimFrame(hModel_[ANIMATION::MOVE], 0, MOVEENDFRAME * 2, 1.0f);
	Model::SetAnimFrame(hModel_[ANIMATION::DEATH], 0, DEATHENDFRAME * 2, 1.0f);


	hModelLow_ = Model::Load("Assets\\Model\\Character\\Enemy-middle.fbx");
	HandleCheck(hModelLow_);

	//SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 2, 0), 3.0f);
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 1.5f, 0), XMFLOAT3(3.0f, 3.0, 3.0f));
	AddCollider(collision);


}

void Enemy::SuperUpdate()
{
	if (!IsEntered()) {
		int currentFrame = Model::GetAnimFrame(hModel_[anim_]);
		Model::SetAnimFrame(hModel_[anim_], currentFrame, currentFrame, 1.0f);
		prevPause_ = true;
	}
	else {
		if (prevPause_) {

			Model::SetAnimFrame(hModel_[anim_], 0, 22 * 2, 1.0f);
			prevPause_ = false;
		}
	}
}

void Enemy::Update()
{
	if (anim_ == ANIMATION::MOVE) {
		if (InvincibleTimer_ <= 0.0f)
			Clash();
		else {
			InvincibleTimer_ -= Time::DeltaTime();
		}

		Move();
	}
	else {
		NonClash();
		if (Model::GetAnimFrame(hModel_[anim_]) >= DEATHENDFRAME * 2) {

			EXPManager* EManager = GetRootJob()->FindGameObject<EXPManager>();
			EManager->SpawnEXP(transform_.position_, status_.exp_);
			KillMe();
		}
	}
}

void Enemy::Move()
{
	isLow_ = true;
	//レイキャスト
	Field* field = GetRootJob()->FindGameObject<Field>();

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		assert(false);

	XMFLOAT3 pPos = player->GetPosition();
	XMFLOAT3 ePos = transform_.position_;

	XMVECTOR pPosVec = XMLoadFloat3(&pPos);
	XMVECTOR ePosVec = XMLoadFloat3(&ePos);

	//プレイヤーと敵の距離のベクトル
	XMVECTOR epDistance = pPosVec - ePosVec;
	//一定範囲内ならしっかりモデルを出現させる
	if (XMVectorGetX(XMVector3Length(epDistance)) <= LOWDISTANCE)
		isLow_ = false;

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

	prePos_ = transform_.position_;

	transform_.position_ += epDistance * status_.speed_ * Time::DeltaTime();

	field->RayCastField(transform_.position_, 3);
}

void Enemy::Draw()
{
	//モデルが前後反転するため一時的に
	Transform tempTrans = transform_;
	tempTrans.rotate_.y += 180;

	for (int i = 0; i < ANIMATION::MAX; i++) {
		Model::SetTransform(hModel_[i], tempTrans);
	}
	Model::SetTransform(hModelLow_, transform_);

	if (!isLow_) {
		Model::Draw(hModel_[anim_]);
	}
	else {
		Model::Draw(hModelLow_);
	}
}

void Enemy::Release()
{
}

void Enemy::HitDamege(int _damege, float _knock)
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	float dBoost = 1.0;
	if (player != nullptr) {
		dBoost = player->GetStatus().strength_;
		if (rand() % 100 < player->GetStatus().critical_) {
			dBoost *= player->GetStatus().criticalBoost_;
		}
	}

	status_.hp_ -= _damege * dBoost;
	InvincibleTimer_ = INVICIBLETIME;
	NonClash();

	if (status_.hp_ <= 0) {
		anim_ = ANIMATION::DEATH;
	}
	else {
		Player* player = GetRootJob()->FindGameObject<Player>();
		if (player == nullptr)
			return;

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
	}
}

int Enemy::CausedDamege()
{
	return status_.power_;
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


