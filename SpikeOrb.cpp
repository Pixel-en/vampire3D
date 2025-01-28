#include "SpikeOrb.h"
#include "EnemySpawn.h"
#include "Player.h"

namespace {
	const int BASEROTATEVALUE{ 1 };//回転の最低値
}

void SpikeOrb::AddBullet()
{
	SpikeOrb::cSpikeOrb* c = Instantiate<SpikeOrb::cSpikeOrb>(GetParent());
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

	AdjustedAngle();
}

void SpikeOrb::AdjustedAngle()
{
	int rot = 360 / List_.size();
	for (int i = 0; i < List_.size(); i++) {
		List_[i]->SetAngle(rot * i);
	}
}

SpikeOrb::SpikeOrb(GameObject* parent)
	:WeaponObject(parent, "SpikeOrb")
{
	for (int i = 0; i < 3; i++) {
		SpikeOrb::cSpikeOrb* c = Instantiate<SpikeOrb::cSpikeOrb>(GetParent());
		List_.push_back(c);
	}

	AdjustedAngle();
}

SpikeOrb::~SpikeOrb()
{
}

void SpikeOrb::Initialize()
{
}

void SpikeOrb::Update()
{
	for (int i = 0; i < List_.size(); i++) {
		if (List_[i]->GetLv() != status_.Lv_)
			List_[i]->SetNextStatus(status_);
	}
}

void SpikeOrb::Draw()
{
}

void SpikeOrb::Release()
{
}

void SpikeOrb::cSpikeOrb::Move()
{
	if (varia_.AttackTime_ < 0.0f) {
		Stop();
		return;
	}
	else {
		varia_.AttackTime_ -= Time::DeltaTime();
	}

	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_.position_ = player->GetPosition();
	Rottransform_.position_ = player->GetPosition();

	Rottransform_.rotate_.y += BASEROTATEVALUE + status_.speed_ * Time::DeltaTime();

	XMMATRIX rotmat = XMMatrixRotationY(Rottransform_.rotate_.y / 180.0f * XM_PI);
	XMVECTOR newpos = XMVector3TransformCoord(XMVectorSet(status_.Range_,0,0,0), rotmat);
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, newpos);
	transform_.position_ = transform_.position_ + pos;

}

void SpikeOrb::cSpikeOrb::ResetSub()
{
	Rottransform_.rotate_.y = angle_;
}

SpikeOrb::cSpikeOrb::cSpikeOrb(GameObject* parent)
	:WeaponObject(parent,"cSpikeOrb"),angle_(0)
{
}

SpikeOrb::cSpikeOrb::~cSpikeOrb()
{
}

void SpikeOrb::cSpikeOrb::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\SpikeOrb.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
	AddCollider(collision);
}

void SpikeOrb::cSpikeOrb::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void SpikeOrb::cSpikeOrb::Release()
{
}