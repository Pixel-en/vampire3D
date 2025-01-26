#include "Knife.h"
#include "EnemySpawn.h"
#include "Player.h"
#include "Engine/Camera.h"
#include <algorithm>

void Knife::AddBullet()
{
	Knife::cKnife* c = Instantiate<Knife::cKnife>(GetParent());
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

}

Knife::Knife(GameObject* parent)
	:WeaponObject(parent, "Knife")
{
	Knife::cKnife* c = Instantiate<Knife::cKnife>(GetParent());
	List_.push_back(c);
}

Knife::~Knife()
{
}

void Knife::Initialize()
{

}

void Knife::Update()
{
	status_.speed_ = status_.Range_ / status_.duration_;
	for (int i = 0; i < List_.size(); i++) {
		if (List_[i]->GetLv() != status_.Lv_)
			List_[i]->SetNextStatus(status_);
	}
}

void Knife::Draw()
{
}

void Knife::Release()
{
}

/*-----------ここからcKnife---------*/

void Knife::cKnife::Move()
{
	if (varia_.AttackTime_ < 0.0f) {
		Stop();
		return;
	}
	else {
		varia_.AttackTime_ -= Time::DeltaTime();
	}

	XMVECTOR pFront = { 0,0,1,0 };
	XMMATRIX rotup = XMMatrixRotationX(XMConvertToRadians(angleX_));
	pFront = XMVector3Normalize(XMVector3TransformCoord(pFront, rotup));

	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//方向ベクトル

	transform_.position_ += dir * status_.speed_ * Time::DeltaTime();

	float distance = transform_.position_ - varia_.originPos_;
	if (distance >= status_.Range_) {
		Stop();
	}
}

void Knife::cKnife::ResetSub()
{
	//カメラアングルについて考えてみて
}

Knife::cKnife::cKnife(GameObject* parent)
	:WeaponObject(parent, "cKnife")
{
}

Knife::cKnife::~cKnife()
{
}

void Knife::cKnife::Initialize()
{

	hModel_ = Model::Load("Assets\\Model\\Knife.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
	AddCollider(collision);
}

void Knife::cKnife::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void Knife::cKnife::Release()
{
}
