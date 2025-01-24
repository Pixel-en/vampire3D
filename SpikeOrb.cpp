#include "SpikeOrb.h"

void SpikeOrb::AddBullet()
{
}

SpikeOrb::SpikeOrb(GameObject* parent)
	:WeaponObject(parent, "SpikeOrb")
{
	SpikeOrb::cSpikeOrb* c = Instantiate<SpikeOrb::cSpikeOrb>(GetParent());
	List_.push_back(c);
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
}

void SpikeOrb::cSpikeOrb::ResetSub()
{
}

SpikeOrb::cSpikeOrb::cSpikeOrb(GameObject* parent)
	:WeaponObject(parent,"cSpikeOrb")
{
}

SpikeOrb::cSpikeOrb::~cSpikeOrb()
{
}

void SpikeOrb::cSpikeOrb::Initialize()
{
	hModel_ = Model::Load("");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
	AddCollider(collision);
}

void SpikeOrb::cSpikeOrb::Draw()
{
}

void SpikeOrb::cSpikeOrb::Release()
{
}

void SpikeOrb::cSpikeOrb::OnCollision(GameObject* pTarget)
{
}