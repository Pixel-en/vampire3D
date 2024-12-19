#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Image.h"

#include "Field.h"
#include <algorithm>

namespace {
	const float MOVESPEED{ 10.0 };
	const float PLAYERHEIGHT{ 1.0f };
	const float ROTATESPEED{ 70.0f };
	const float RAYHEIGHT{ 5.0f };
	const float GRAVITY{ 9.8f / 2.0f / 60.0f };
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"), hModel_(-1),hImage_(-1)
{
	transform_.position_ = { 0,50,0 };
	lookHeight_ = PLAYERHEIGHT;
	onGround_ = false;
	gravity = 0.0f;
	crossTrans = transform_;
	crossTrans.position_ = { 0,0,0 };
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Player.fbx");
	assert(hModel_ >= 0);

	hImage_ = Image::Load("Assets\\Image\\Test_Crosshair.png");
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

	XMVECTOR Gravity = XMVECTOR{ 0,0,0 };
	//重力方向のベクトルを作る
	if (!onGround_) {
		gravity += GRAVITY;
		Gravity = XMVECTOR{ 0,-1,0 };
	}
	else {
		Gravity = XMVECTOR{ 0,0,0 };
		gravity = 0.0f;
	}

	Gravity = XMVector3Normalize(Gravity);

	//移動
	if (Input::IsKey(DIK_W))
		moveVec = XMVectorSetZ(moveVec, 1.0);
	if (Input::IsKey(DIK_S))
		moveVec = XMVectorSetZ(moveVec, -1.0);
	if (Input::IsKey(DIK_A))
		moveVec = XMVectorSetX(moveVec, -1.0);
	if (Input::IsKey(DIK_D))
		moveVec = XMVectorSetX(moveVec, 1.0);

	//カメラ回転
	if (Input::IsKey(DIK_LEFT))
		transform_.rotate_.y -= ROTATESPEED * Time::DeltaTime();
	if (Input::IsKey(DIK_RIGHT))
		transform_.rotate_.y += ROTATESPEED * Time::DeltaTime();

	//カメラ縦
	if (Input::IsKey(DIK_UP))
		lookHeight_ += 1.0f * Time::DeltaTime();
	if (Input::IsKey(DIK_DOWN))
		lookHeight_ -= 1.0f * Time::DeltaTime();
	lookHeight_ = std::clamp(lookHeight_, 0.0f, 2.0f);

	//Y軸の回転をマトリクスに変換
	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	//移動方向ベクトルに回転マトリクスをかけ回転させたベクトルを作る
	XMVECTOR rotMoveVec = XMVector3TransformCoord(moveVec, rot);
	rotMoveVec = XMVector3Normalize(rotMoveVec);

	//ターゲット用ベクトルに回転マトリクスをかけ回転させたベクトルを作る
	XMVECTOR rotCamtarVec = XMVector3TransformCoord(camtarVec, rot);
	rotCamtarVec = XMVector3Normalize(rotCamtarVec);

	transform_.position_ += rotMoveVec * MOVESPEED * Time::DeltaTime() + Gravity * gravity;


	//フィールドからモデルのハンドルをとってくる
	Field* field = GetParent()->FindGameObject<Field>();
	//レイがあたった距離下げる
	transform_.position_.y -= field->GetRayDist();

	//カメラ
	if (Input::IsKey(DIK_SPACE)) {
		//俯瞰モード
		Camera::SetPosition({ transform_.position_.x,transform_.position_.y + 15,transform_.position_.z - 10 });
		Camera::SetTarget(transform_.position_);
	}
	else {
		//一人称モード
		Camera::SetPosition({ transform_.position_.x,transform_.position_.y + PLAYERHEIGHT,transform_.position_.z });
		XMFLOAT3 tarPos = transform_.position_ + 1.0f * rotCamtarVec;
		Camera::SetTarget({ tarPos.x, tarPos.y + lookHeight_, tarPos.z });
	}

}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	if (Input::IsKey(DIK_SPACE)) 
		Model::Draw(hModel_);
	Image::SetTransform(hImage_, crossTrans);
	Image::Draw(hImage_);
}

void Player::Release()
{
}

XMFLOAT3 Player::GetRayStart()
{
	return { transform_.position_.x,transform_.position_.y + RAYHEIGHT,transform_.position_.z };
}

float Player::GetRayHeight()
{
	return RAYHEIGHT;
}
