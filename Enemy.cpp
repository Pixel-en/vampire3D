#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include <algorithm>

#include "Player.h"
#include "Field.h"

namespace {
	float MOVESPEED{ 5.0f };
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_(-1)
{
	status_.speed_ = MOVESPEED;
	transform_.position_ = { 0,0,0 };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Enemy.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 3.0f);
	AddCollider(collision);
}

void Enemy::Update()
{
	Move();
}

void Enemy::Move()
{
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

	transform_.position_ += epDistance * status_.speed_ * Time::DeltaTime();

	field->RayCastField(transform_.position_, 3);
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}

