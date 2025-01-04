#include "Knife.h"

void Knife::Move()
{
	XMVECTOR pFront = { 0,0,1,0 };

	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//•ûŒüƒxƒNƒgƒ‹

	transform_.position_ += dir * status_.speed_ * Time::DeltaTime();

	float distance = transform_.position_ - originPos;
	if (distance >= 100.0f)
		KillMe();
}

Knife::Knife(GameObject* parent)
	:WeaponObject(parent,"Knife")
{
}

Knife::~Knife()
{
}

void Knife::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Box.fbx");
	assert(hModel_ >= 0);

	Player* player = GetRootJob()->FindGameObject<Player>();
	assert(player != nullptr);

	transform_ = player->GetTransform();
	originPos = player->GetPosition();
}

void Knife::Update()
{
	Move();

}

void Knife::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Knife::Release()
{
}
