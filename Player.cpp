#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

namespace {
	const float MOVESPEED{ 10.0 };
}

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
	//Œü‚¢‚Ä‚¢‚é•ûŒü‚ÌƒxƒNƒgƒ‹
	XMVECTOR moveVec = XMVECTOR{ 0,0,0 };
	if (Input::IsKey(DIK_UP))
		moveVec = XMVectorSetZ(moveVec, 1.0);
	if (Input::IsKey(DIK_DOWN))
		moveVec = XMVectorSetZ(moveVec, -1.0);
	if (Input::IsKey(DIK_LEFT))
		moveVec = XMVectorSetX(moveVec, -1.0);
	if (Input::IsKey(DIK_RIGHT)) 
		moveVec=XMVectorSetX(moveVec, 1.0);
	//if (Input::IsKey(DIK_A))
	//	transform_.rotate_.y -= 50.0f * Time::DeltaTime();
	//if (Input::IsKey(DIK_D))
	//	transform_.rotate_.y += 50.0f * Time::DeltaTime();

	transform_.position_ += moveVec * MOVESPEED * Time::DeltaTime();
}

void Player::Draw()
{
	Model::SetTransform(hmodel, transform_);
	Model::Draw(hmodel);
}

void Player::Release()
{
}
