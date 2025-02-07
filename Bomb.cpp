#include "Bomb.h"

void Bomb::AddBullet()
{
}

Bomb::Bomb(GameObject* parent)
	:WeaponObject(parent,"Bomb")
{
}

Bomb::~Bomb()
{
}

void Bomb::Initialize()
{
}

void Bomb::Update()
{
	if (varia_.ReStartTimer_ < 0.0f) {
		//for(int i=0;i<)
	}
	else {
		varia_.ReStartTimer_ -= Time::DeltaTime();
	}
}

void Bomb::Draw()
{
}

void Bomb::Release()
{
}


void Bomb::cBomb::Move()
{
}

void Bomb::cBomb::ResetSub()
{
}

Bomb::cBomb::cBomb(GameObject* parent)
	:WeaponObject(parent,"cBomb")
{
}

Bomb::cBomb::~cBomb()
{
}

void Bomb::cBomb::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Bomb.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
	AddCollider(collision);
}

void Bomb::cBomb::Update()
{
}

void Bomb::cBomb::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bomb::cBomb::Release()
{
}