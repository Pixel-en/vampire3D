#include "Knife.h"
#include "EnemySpawn.h"
#include "Player.h"

Knife::Knife(GameObject* parent)
	:WeaponObject(parent,"Knife")
{
	Knife::cKnife* c = Instantiate<Knife::cKnife>(GetParent());
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
	for (int i = 0; i < List_.size(); i++) {
		if (nextStatus_.Lv_ != List_[i]->GetLv()) {
			//List_[i].
		}
	}
}

void Knife::Draw()
{
}

void Knife::Release()
{
}

/*-----------‚±‚±‚©‚çcKnife---------*/

void Knife::cKnife::Move()
{
	XMVECTOR pFront = { 0,0,1,0 };

	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//•ûŒüƒxƒNƒgƒ‹

	transform_.position_ += dir * status_.speed_ * Time::DeltaTime();

	float distance = transform_.position_ - varia_.originPos_;
	if (distance >= status_.Range_) {
		Stop();
	}
}

void Knife::cKnife::ResetSub()
{
	//status_ = nextStatus_;
}

Knife::cKnife::cKnife(GameObject* parent)
	:WeaponObject(parent,"cKnife")
{
}

Knife::cKnife::~cKnife()
{
}

void Knife::cKnife::Initialize()
{
	Reset();

	hModel_ = Model::Load("Assets\\Model\\Knife.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
	AddCollider(collision);


	status_.speed_ = status_.Range_ / status_.duration_;
	nextStatus_ = status_;
}

void Knife::cKnife::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	
}

void Knife::cKnife::Release()
{
}

void Knife::cKnife::OnCollision(GameObject* pTarget)
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
