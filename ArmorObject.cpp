#include "ArmorObject.h"

ArmorObject::ArmorObject(GameObject* parent)
	:GameObject(parent, "ArmorObject")
{
}

ArmorObject::ArmorObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
}

ArmorObject::~ArmorObject()
{
}

void ArmorObject::Initialize()
{
}

void ArmorObject::Update()
{
}

void ArmorObject::Draw()
{
}

void ArmorObject::Release()
{
}
