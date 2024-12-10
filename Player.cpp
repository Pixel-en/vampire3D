#include "Player.h"
#include "Engine/Model.h"

Player::Player(GameObject* parent)
	:hmodel(-1)
{
	transform_.position_ = { 0,0,0 };
}

Player::~Player()
{
}

void Player::Initialize()
{
	hmodel = Model::Load("Assets\\Model\\Player.fbx");
	assert(hmodel >= 0);
}

void Player::Update()
{
}

void Player::Draw()
{
	Model::SetTransform(hmodel, transform_);
	Model::Draw(hmodel);
}

void Player::Release()
{
}
