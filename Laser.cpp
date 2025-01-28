#include "Laser.h"

void Laser::AddBullet()
{
	Laser::cLaser* c = Instantiate<Laser::cLaser>(GetParent());
	c->SetStatus(status_);
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
	if (varia_.AttackTime_ < 0.0f) {
		Stop();
		return;
	}
	else {
		varia_.AttackTime_ -= Time::DeltaTime();
	}



}

void Laser::cLaser::ResetSub()
{
	Transform laserStart_ = transform_;
	XMVECTOR pFront = { 0,0,1,0 };

	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//方向ベクトル

	laserStart_.position_ += dir * 2;
	transform_.position_ = laserStart_.position_ + dir * (status_.Range_ / 2.0f);

	transform_.scale_.z *= status_.Range_;
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

	//コライダー
}

void Laser::cLaser::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Laser::cLaser::Release()
{
}