#include "Enemy.h"
#include "Engine/Model.h"

#include "Player.h"
#include "Field.h"

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Player.fbx");
	assert(hModel_ >= 0);
}

void Enemy::Update()
{
	Field* f = GetRootJob()->FindGameObject<Field>();
	f->RayCastField(transform_.position_, 3);

	Player* p = GetRootJob()->FindGameObject<Player>();
	if (p == nullptr)
		assert(false);

	XMFLOAT3 pPos = p->GetPosition();
	XMFLOAT3 ePos = transform_.position_;

	XMVECTOR pVec = XMLoadFloat3(&pPos);
	XMVECTOR eVec = XMLoadFloat3(&ePos);

	XMVECTOR eFront = { 0,0,0.2f,0 };
	XMMATRIX mat = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR eRotFront = XMVector3TransformCoord(eFront, mat);
	XMVECTOR epVec = pVec - eVec;
	eRotFront = XMVector3Normalize(eRotFront);
	epVec = XMVector3Normalize(epVec);

	float angle = XMVectorGetX(XMVector3AngleBetweenNormals(eRotFront, epVec));
	Debug::Log(XMConvertToDegrees( angle), true);
	transform_.rotate_.y += XMConvertToDegrees(angle);
	//float dot = XMVectorGetX(XMVector3Dot(epVec, eRotFront));	//内積
	//float angle = acos(dot);	//ラジアン角
	//Debug::Log(angle, true);
	//XMMATRIX mat = XMMatrixRotationY(angle);
	//XMVECTOR rotVec = XMVector3TransformCoord(XMVectorSet(0, 0, 0.2f, 0), mat);
	//rotVec = XMVector3Normalize(rotVec);	//回転ベクトル
	//float epX = XMVectorGetX(epVec);
	//if ((epX >= 0.0) - (epX < 0.0) == 1)
	//	transform_.rotate_.y += XMConvertToDegrees(angle);
	//else
	//	transform_.rotate_.y += -XMConvertToDegrees(angle);

	//transform_.position_ += epVec * 50.0f * Time::DeltaTime();
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
