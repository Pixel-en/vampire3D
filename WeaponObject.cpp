#include "WeaponObject.h"
#include "Player.h"

namespace {
	const float ATTACKTIME{ 1.0f };
}

void WeaponObject::Reset()
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_ = player->GetTransform();
	originPos = player->GetPosition();
	attackTimer_ = ATTACKTIME;

	Visible();
	Clash();
	allowsMove_ = true;
}

void WeaponObject::Stop()
{
	Invisible();
	NonClash();
	allowsMove_ = false;
}

WeaponObject::WeaponObject(GameObject* parent)
	:GameObject(parent, ""), hModel_(-1),allowsMove_(true)
{
	attackTimer_ = ATTACKTIME;
}

WeaponObject::WeaponObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name),hModel_(-1),allowsMove_(true)
{
}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::Initialize()
{
	Reset();
}

void WeaponObject::Update()
{
	if (attackTimer_ < 0.0f) {
		Reset();
	}
	else {
		attackTimer_ -= Time::DeltaTime();
	}

	Move();
}

void WeaponObject::Draw()
{
}

void WeaponObject::Release()
{
}

void WeaponObject::OnCollision(GameObject* pTarget)
{
}
