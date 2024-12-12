#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Image.h"

namespace {
	const float MOVESPEED{ 10.0 };
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"), hmodel(-1), hImage_(-1)
{
	transform_.position_ = { 0,0,0 };
	cursorTrans_.position_ = { 0,0,0 };
}

Player::~Player()
{
}

void Player::Initialize()
{
	hmodel = Model::Load("Assets\\Model\\Player.fbx");
	assert(hmodel >= 0);
	hImage_ = Image::Load("Assets\\Image\\cursor.png");
	assert(hImage_ >= 0);
}

void Player::Update()
{
	Move();
}

void Player::Move()
{
	//移動方向ベクトル
	XMVECTOR moveVec = XMVECTOR{ 0,0,0 };
	//カメラターゲット用ベクトル
	XMVECTOR camtarVec = XMVECTOR{ 0, 0, 1, 0 };

	cursorTrans_.position_ = { Input::GetMousePosition().x - screenWidth / 2.0f ,Input::GetMousePosition().y - screenHeight / 2.0f,Input::GetMousePosition().z };
	Debug::Log(cursorTrans_.position_.x, true);

	//移動
	if (Input::IsKey(DIK_W))
		moveVec = XMVectorSetZ(moveVec, 1.0);
	if (Input::IsKey(DIK_S))
		moveVec = XMVectorSetZ(moveVec, -1.0);
	if (Input::IsKey(DIK_A))
		moveVec = XMVectorSetX(moveVec, -1.0);
	if (Input::IsKey(DIK_D))
		moveVec = XMVectorSetX(moveVec, 1.0);

	if (Input::IsKeyUp(DIK_L))
		cursorTrans_.position_.x += 1;
	if (Input::IsKeyUp(DIK_J))
		cursorTrans_.position_.x -= 1;
	//カメラ回転
	if (Input::IsKey(DIK_LEFT))
		transform_.rotate_.y -= 50.0f * Time::DeltaTime();
	if (Input::IsKey(DIK_RIGHT))
		transform_.rotate_.y += 50.0f * Time::DeltaTime();

	//Y軸の回転をマトリクスに変換
	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	//移動方向ベクトルに回転マトリクスをかけ回転させたベクトルを作る
	XMVECTOR rotMoveVec = XMVector3TransformCoord(moveVec, rot);
	rotMoveVec = XMVector3Normalize(rotMoveVec);

	//ターゲット用ベクトルに回転マトリクスをかけ回転させたベクトルを作る
	XMVECTOR rotCamtarVec = XMVector3TransformCoord(camtarVec, rot);
	rotCamtarVec = XMVector3Normalize(rotCamtarVec);
	
	transform_.position_ += rotMoveVec * MOVESPEED * Time::DeltaTime();

	if (Input::IsKey(DIK_SPACE)) {
		Camera::SetPosition({ transform_.position_.x,transform_.position_.y + 15,transform_.position_.z - 0.1f });
		Camera::SetTarget(transform_.position_);
	}
	else {
		Camera::SetPosition({ transform_.position_.x,transform_.position_.y+1,transform_.position_.z });
		Camera::SetTarget(transform_.position_ + 10.0f * rotCamtarVec);
	}
}

void Player::Draw()
{
	Model::SetTransform(hmodel, transform_);
	if (Input::IsKey(DIK_SPACE)) 
		Model::Draw(hmodel);
	Image::SetTransform(hImage_, cursorTrans_);
	Image::Draw(hImage_);
}

void Player::Release()
{
}
