#include "Apple.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Player.h"

namespace {
	const float DISMAX{ 20.0f };	//プレイヤーとの距離の最大値
	const float RATE{0.2f };	//回復割合
	const float MOVESPEED{ 50.0f };
}

Apple::Apple(GameObject* parent)
	:GameObject(parent,"Apple"),hModel_(-1),speed_(1.0f)
{
}

Apple::~Apple()
{
}

void Apple::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Apple.fbx");
	HandleCheck(hModel_, "リンゴのモデルがない");

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1.0f, 0), 1.5f);
	AddCollider(collision);

	speed_ = MOVESPEED;
}

void Apple::Update()
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

	transform_.position_ += direction * speed_ * Time::DeltaTime();
}

void Apple::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Apple::Release()
{
}

void Apple::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		Player* player = GetParent()->FindGameObject<Player>();
		player->HealingHp(player->GetStatus().maxHp_ * RATE);
		KillMe();
	}
}
