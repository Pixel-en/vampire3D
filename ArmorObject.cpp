#include "ArmorObject.h"

ArmorObject::ArmorObject(GameObject* parent)
	:GameObject(parent, "ArmorObject")
{
}

ArmorObject::ArmorObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
	numLevel_ = 0;;
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
