#include "EXP.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

EXP::EXP(GameObject* parent)
	:GameObject(parent,"EXP"),hModel_(-1)
{
}

EXP::~EXP()
{
}

void EXP::Initialize()
{
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f);
	AddCollider(collision);
}

void EXP::Update()
{
}

void EXP::Draw()
{
}

void EXP::Release()
{
}
