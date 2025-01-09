#include "EXP.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Player.h"

EXP::EXP(GameObject* parent)
	:GameObject(parent,"EXP"),hModel_(-1)
{
	dead_ = false;
	expValue_ = 0;
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
	//ˆê’è”ÍˆÍ“à‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚½‚ç‹ß‚Ã‚­
}

void EXP::Draw()
{
}

void EXP::Release()
{
}

void EXP::SetStatus(XMFLOAT3 _pos, int _exp)
{
	transform_.position_ = _pos;
	expValue_ = _exp;
}

void EXP::AddEXP(int _exp)
{
	expValue_ += _exp;
}

void EXP::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		Player* player = GetParent()->FindGameObject<Player>();
		player->AcquisitionEXP(expValue_);
		dead_ = true;
	}
}
