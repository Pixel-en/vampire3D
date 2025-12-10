#include "EXP.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Player.h"
#include "Field.h"
#include "Knife.h"

namespace {
	const float DISMAX{ 20.0f };
	const float MOVESPEED{ 70.0f };
	const int ANIMENDFRAME{ 120 };
}

void EXP::LoadModel()
{
	if (expValue_ <= SMALL) {
		hModel_ = Model::Load("Assets\\Model\\EXP_Blue.fbx");

	}
	else if (expValue_ <= MEDIUM) {
		hModel_ = Model::Load("Assets\\Model\\EXP_Yellow.fbx");
	}
	else if (expValue_ <= LARGE) {
		hModel_ = Model::Load("Assets\\Model\\EXP_Green.fbx");
	}
	else {
		hModel_ = Model::Load("Assets\\Model\\EXP_Red.fbx");
	}

	HandleCheck(hModel_);

	Model::SetAnimFrame(hModel_, 0, ANIMENDFRAME, 1.0f);
}

EXP::EXP(GameObject* parent)
	:GameObject(parent, "EXP"), hModel_(-1)
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


	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1.0f, 0), 1.0f);
	AddCollider(collision);
}

void EXP::Update()
{

	//一定範囲内にプレイヤーがいたら近づく
	Player* player = GetParent()->FindGameObject<Player>();
	XMFLOAT3 pPos = player->GetPosition();
	float distance = transform_.position_ - pPos;

	if (distance > DISMAX * player->GetStatus().collectionRange_)
		return;

	XMVECTOR pPosVec = XMLoadFloat3(&pPos);
	XMVECTOR PosVec = XMLoadFloat3(&transform_.position_);

	XMVECTOR direction = pPosVec - PosVec;

	direction = XMVector3Normalize(direction);

	//近づく
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
	//地面に埋まらないように+1
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
	//プレイヤーと衝突したら経験値を加算して消える
	if (pTarget->GetObjectName() == "Player") {
		Player* player = GetParent()->FindGameObject<Player>();
		player->AcquisitionEXP(expValue_);
		dead_ = true;
		NonClash();
	}
}
