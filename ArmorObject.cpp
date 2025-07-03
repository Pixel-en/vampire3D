#include "ArmorObject.h"

ArmorObject::ArmorObject(GameObject* parent)
	:GameObject(parent, "ArmorObject")
{
	BeBuff_ = 1.0f;
	BeDBuff_ = 1.0f;
}

ArmorObject::ArmorObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
	numLevel_ = 0;
	BeBuff_ = 1.0f;
	BeDBuff_ = 1.0f;
	Invisible();
	Leave();
}

ArmorObject::~ArmorObject()
{
}

void ArmorObject::Initialize()
{
}

void ArmorObject::Update()
{
	exit(0);
}

void ArmorObject::Draw()
{
}

void ArmorObject::Release()
{
}
