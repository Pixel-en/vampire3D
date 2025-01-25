#include "SpikeOrb.h"
#include "EnemySpawn.h"
#include "Player.h"

namespace {
	const float CIRCLERANGE{ 5.0f };
}

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
	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_.position_ = player->GetPosition();
	//transform_.position_.x += CIRCLERANGE;
	Rottransform_.position_ = player->GetPosition();

	Rottransform_.rotate_.y += 1 + status_.speed_ * Time::DeltaTime();

	//Rottransform_.rotate_.y = 90;

	XMVECTOR pos = XMLoadFloat3(&transform_.position_);
	XMMATRIX rotmat = XMMatrixRotationY(Rottransform_.rotate_.y / 180.0f * XM_PI);
	XMVECTOR newpos = XMVector3TransformCoord(XMVectorSet(5,0,0,0), rotmat);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, newpos);
	transform_.position_ = {transform_.position_.x+temp.x,transform_.position_.y + temp.y ,transform_.position_.z + temp.z};

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

void SpikeOrb::cSpikeOrb::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
		std::vector<Enemy*> List = ep->GetEnemyList();
		for (int i = 0; i < List.size(); i++) {
			if (dynamic_cast<Enemy*>(pTarget)->GetEnemyNumber() == List[i]->GetEnemyNumber()) {
				List[i]->HitDamege(status_.damege_);
				break;
			}
		}
	}
}