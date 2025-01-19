#include "Knife.h"
#include "EnemySpawn.h"
#include "Player.h"

namespace {
	const float ATTACKDISTANCE{ 50.0f };
	const float ATTACKTIME{ 1.0f };
}

Knife::Knife(GameObject* parent)
	:WeaponObject(parent,"Knife")
{
	cKnife* c = Instantiate<cKnife>(GetParent());
	List_.push_back(c);
}

Knife::~Knife()
{
}

void Knife::Initialize()
{
	nextStatus_ = status_;
}

void Knife::Update()
{
}

void Knife::Draw()
{
}

void Knife::Release()
{
}

/*-----------‚±‚±‚©‚çcKnife---------*/

void cKnife::Move()
{
	XMVECTOR pFront = { 0,0,1,0 };

	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//•ûŒüƒxƒNƒgƒ‹

	transform_.position_ += dir * status_.speed_ * Time::DeltaTime();

	float distance = transform_.position_ - varia_.originPos_;
	if (distance >= ATTACKDISTANCE) {
		Stop();
	}
}

void cKnife::ResetSub()
{
	status_ = nextStatus_;
}

cKnife::cKnife(GameObject* parent)
	:WeaponObject(parent,"cKnife")
{
}

cKnife::~cKnife()
{
}

void cKnife::Initialize()
{
	Reset();

	hModel_ = Model::Load("Assets\\Model\\Knife.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
	AddCollider(collision);
	status_.damege_ = 5;
	status_.hp_ = 5;
	status_.speed_ = ATTACKDISTANCE / ATTACKTIME;
	nextStatus_ = status_;
}

void cKnife::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	
}

void cKnife::Release()
{
}

void cKnife::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
		std::vector<Enemy*> List = ep->GetEnemyList();
		for (int i = 0; i < List.size(); i++) {
			if (dynamic_cast<Enemy*>(pTarget)->GetEnemyNumber() == List[i]->GetEnemyNumber()) {
				List[i]->HitDamege(status_.damege_);
				Penetration();
				break;
			}
		}
	}
}
