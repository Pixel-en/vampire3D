#include "Enemy.h"


Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"")
{
}

Enemy::Enemy(GameObject* parent, const std::string& name)
	:GameObject(parent,name)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

void Enemy::Release()
{
}
