#include "WeaponObject.h"

WeaponObject::WeaponObject(GameObject* parent)
	:GameObject(parent, ""), hModel_(-1)
{
}

WeaponObject::WeaponObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name),hModel_(-1)
{
}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::Initialize()
{
}

void WeaponObject::Update()
{
}

void WeaponObject::Draw()
{
}

void WeaponObject::Release()
{
}
