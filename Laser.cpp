#include "Laser.h"

namespace {
	const float LASERSTARTSIZE{ 0.05f };
	const float LASERSIZERATE{ 0.2f };
	const float LASERANGLERATE{ 2.0f };
	const float LASERWAITTIME{ 0.5f };
}

void Laser::AddBullet()
{
	Laser::cLaser* c = Instantiate<Laser::cLaser>(GetParent());
	c->SetStatus(status_);
	c->SetCollider();
	//止まっているなら残りの時間+バッファ
	if (!List_[0]->isMove()) {
		c->SetResetTimer(List_[0]->GetResetTimer() + (List_.size() * BUFFER));
	}
	//動いているなら残りの攻撃持続時間+リセットタイム+バッファ
	else {
		c->SetResetTimer(List_[0]->GetResetTimer() + List_[0]->GetAttackTimer() + (List_.size() * BUFFER));
	}
	List_.push_back(c);
	LaserDir();
}

void Laser::LaserDir()
{
	int rot = 360 / List_.size();
	for (int i = 0; i < List_.size(); i++) {
		List_[i]->SetAngle(rot * i);
	}
}

Laser::Laser(GameObject* parent)
	:WeaponObject(parent, "Laser")
{
	Laser::cLaser* c = Instantiate<Laser::cLaser>(GetParent());
	c->SetStatus(status_);
	c->SetCollider();
	List_.push_back(c);
}

Laser::~Laser()
{
}

void Laser::Initialize()
{
}

void Laser::Update()
{
	for (int i = 0; i < List_.size(); i++) {
		if (List_[i]->GetLv() != status_.Lv_)
			List_[i]->SetNextStatus(status_);
	}
}

void Laser::Draw()
{
}

void Laser::Release()
{
}


void Laser::cLaser::Move()
{
	//演出のためレーザーを回す
	transform_.rotate_.z -= LASERANGLERATE;

	//大きくなるまでのタイマー
	if (BiggerWaittimer_ < 0.0) {
		Clash();

		//徐々に大きく
		if (transform_.scale_.x < status_.size_)
			transform_.scale_ += {LASERSIZERATE, LASERSIZERATE, 0};
		else {
			transform_.scale_ = { status_.size_,status_.size_,transform_.scale_.z };

			//攻撃持続タイマー
			if (varia_.AttackTime_ < 0.0f) {
				Stop();
				return;
			}
			else {
				varia_.AttackTime_ -= Time::DeltaTime();
			}
		}
	}
	else {
		BiggerWaittimer_ -= Time::DeltaTime();
		NonClash();
	}
}

void Laser::cLaser::ResetSub()
{
	Transform laserStart_ = transform_;
	XMVECTOR pFront = { 0,0,1,0 };
	//プレイヤーの向いている方向に回転
	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//方向ベクトル
	//スタート位置を決める
	laserStart_.position_ += dir * 2;
	//オブジェクトの座標を先端と後端の中点にする
	transform_.position_ = laserStart_.position_ + dir * (status_.Range_ / 2.0f);
	//大きさを攻撃距離倍する
	transform_.scale_ = { LASERSTARTSIZE,LASERSTARTSIZE, transform_.scale_.z * status_.Range_ };
	//レーザーの回る角度を0にする
	transform_.rotate_.z = 0;
	//レーザーの拡大開始するまでのタイマー
	BiggerWaittimer_ = LASERWAITTIME;

	//レーダーのコライダーの位置を合わせる
	int count = 0;
	for (auto itr = colliderList_.begin(); itr != colliderList_.end(); itr++) {
		//ワールド座標にするためにtransform_を引く
		//transform_-レーダーのスタート+レーダーの端合わせ+(方向ベクトル*位置合わせカウント*大きさ*バッファ(直径))
		(*itr)->SetPosition(Transform::Float3Sub(transform_.position_, laserStart_.position_ + dir + (dir * count * status_.size_ * (status_.size_ * 2))));
		count++;
	}
}

Laser::cLaser::cLaser(GameObject* parent)
	:WeaponObject(parent, "cLaser")
{
}

Laser::cLaser::~cLaser()
{
}

void Laser::cLaser::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Laser.fbx");
	assert(hModel_ >= 0);

	targetName_ = "Enemy";
}

void Laser::cLaser::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Laser::cLaser::Release()
{
}

void Laser::cLaser::SetCollider()
{
	ClearCollider();

	for (int i = 0; i < status_.Range_ / 2; i++) {
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
		AddCollider(collision);
	}
}
