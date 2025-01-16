#include "WeaponObject.h"
#include "Player.h"

namespace {
	const float RESTARTTIME{ 1.0f };
}

void WeaponObject::Reset()
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_ = player->GetTransform();
	originPos = player->GetPosition();
	ReStartTimer_ = RESTARTTIME;

	Visible();
	Clash();
	allowsMove_ = true;

	status_.damege_ = 5;
	status_.hp_ = 1;
	status_.speed_ = 20.0f;

	ResetSub();
}

void WeaponObject::Stop()
{
	Invisible();
	NonClash();
	allowsMove_ = false;
}

void WeaponObject::Penetration()
{
	status_.hp_--;
	if (status_.hp_ <= 0)
		Stop();
}

WeaponObject::WeaponObject(GameObject* parent)
	:GameObject(parent, ""), hModel_(-1),allowsMove_(true)
{
	ReStartTimer_ = RESTARTTIME;
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
	if(allowsMove_)
		Move();
	else {
		if (ReStartTimer_ < 0.0f) {
			Reset();
		}
		else {
			ReStartTimer_ -= Time::DeltaTime();
		}
	}

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
