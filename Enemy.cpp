#include "Enemy.h"
#include "Engine/Model.h"

#include "Player.h"

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Box.fbx");
	assert(hModel_ >= 0);
}

void Enemy::Update()
{
	Player* p = GetRootJob()->FindGameObject<Player>();

	//Šp“xo‚µ‚ÄŒü‚¯‚é
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
