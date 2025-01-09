#include "Knife.h"
#include "EnemySpawn.h"
#include "Enemy.h"
#include "EXPManager.h"

void Knife::Move()
{

	if (!allowsMove_)
		return;

	XMVECTOR pFront = { 0,0,1,0 };

	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	XMVECTOR dir = XMVector3TransformCoord(pFront, rot);
	dir = XMVector3Normalize(dir);	//•ûŒüƒxƒNƒgƒ‹

	transform_.position_ += dir * status_.speed_ * Time::DeltaTime();

	float distance = transform_.position_ - originPos;
	if (distance >= 100.0f) {
		Stop();
	}
}

Knife::Knife(GameObject* parent)
	:WeaponObject(parent,"Knife")
{
	status_.speed_ = 50.0f;
}

Knife::~Knife()
{
}

void Knife::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Knife.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
	AddCollider(collision);
}


void Knife::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Knife::Release()
{
}

void Knife::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
		std::vector<Enemy*> List = ep->GetEnemyList();
		for (int i = 0; i < List.size(); i++) {
			if (dynamic_cast<Enemy*>(pTarget)->GetEnemyNumber() == List[i]->GetEnemyNumber()) {

				EXPManager* EManager = GetRootJob()->FindGameObject<EXPManager>();
				EManager->SpawnEXP(transform_.position_, 2);

				List[i]->KillMe();
				pTarget->KillMe();
				Stop();
				break;
			}
		}
	}
}