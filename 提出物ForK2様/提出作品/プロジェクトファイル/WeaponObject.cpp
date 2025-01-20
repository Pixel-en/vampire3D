#include "WeaponObject.h"
#include "Player.h"

namespace {
	const float RESTARTTIME{ 1.0f };
}

void WeaponObject::Reset()
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_ = player->GetTransform();
	varia_.originPos_ = player->GetPosition();
	varia_.ReStartTimer_ = RESTARTTIME;

	Visible();
	Clash();
	varia_.allowsMove_ = true;
	varia_.peneCount_ = status_.hp_;

	ResetSub();
}

void WeaponObject::Stop()
{
	Invisible();
	NonClash();
	varia_.allowsMove_ = false;
}

void WeaponObject::Penetration()
{
	varia_.peneCount_--;
	if (varia_.peneCount_ <= 0)
		Stop();
}

WeaponObject::WeaponObject(GameObject* parent)
	:GameObject(parent, ""), hModel_(-1)
{
	varia_.allowsMove_ = true;
	varia_.ReStartTimer_ = RESTARTTIME;

	status_.Lv_ = 1;
	status_.damege_ = 1;
	status_.hp_ = 1;
	status_.speed_ = 20.0f;
	nextStatus_ = status_;

}

WeaponObject::WeaponObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name),hModel_(-1)
{
	varia_.allowsMove_ = true;
	varia_.ReStartTimer_ = RESTARTTIME;

	status_.Lv_ = 1;
	status_.damege_ = 1;
	status_.hp_ = 1;
	status_.speed_ = 20.0f;
	nextStatus_ = status_;
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
	if (varia_.allowsMove_)
		Move();
	else {
		if (varia_.ReStartTimer_ < 0.0f) {
			Reset();
		}
		else {
			varia_.ReStartTimer_ -= Time::DeltaTime();
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

void WeaponObject::LevelUp(std::string str)
{
}
