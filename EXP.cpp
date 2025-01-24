#include "EXP.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Player.h"
#include "Field.h"
#include "Knife.h"

namespace {
	const float DISMAX{ 10.0f };
	const float MOVESPEED{ 70.0f };
}

void EXP::LoadModel()
{

	if (expValue_ <= SMALL) {
		hModel_ = Model::Load("Assets\\Model\\EXP_Blue.fbx");
	}
	else if(expValue_ <= MEDIUM) {
		hModel_ = Model::Load("Assets\\Model\\EXP_Yellow.fbx");
	}
	else if (expValue_ <= LARGE) {
		hModel_ = Model::Load("Assets\\Model\\EXP_Green.fbx");
	}
	else {
		hModel_ = Model::Load("Assets\\Model\\EXP_Red.fbx");
	}
	assert(hModel_ >= 0);
}

EXP::EXP(GameObject* parent)
	:GameObject(parent,"EXP"),hModel_(-1)
{
	dead_ = false;
	expValue_ = 0;
	speed_ = MOVESPEED;
}

EXP::~EXP()
{
}

void EXP::Initialize()
{
	LoadModel();

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0.5f, 0), 0.5f);
	AddCollider(collision);
}

void EXP::Update()
{
	//ˆê’è”ÍˆÍ“à‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚½‚ç‹ß‚Ã‚­
	Player* player = GetParent()->FindGameObject<Player>();
	XMFLOAT3 pPos = player->GetPosition();
	float distance = transform_.position_ - pPos;

	if (distance > DISMAX)
		return;
	XMVECTOR pPosVec = XMLoadFloat3(&pPos);
	XMVECTOR PosVec = XMLoadFloat3(&transform_.position_);

	XMVECTOR direction = pPosVec - PosVec;

	direction = XMVector3Normalize(direction);

	transform_.position_ += direction * speed_ * Time::DeltaTime();

}

void EXP::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void EXP::Release()
{
}

void EXP::SetStatus(XMFLOAT3 _pos, int _exp)
{
	transform_.position_ = _pos;
	transform_.position_.y += 1;
	expValue_ = _exp;
	LoadModel();

	Field* field = GetParent()->FindGameObject<Field>();
	field->RayCastField(transform_.position_, 5);
}

void EXP::AddEXP(int _exp)
{
	expValue_ += _exp;
	LoadModel();
}

void EXP::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		Player* player = GetParent()->FindGameObject<Player>();
		player->AcquisitionEXP(expValue_);
		dead_ = true;
		NonClash();
	}
}
