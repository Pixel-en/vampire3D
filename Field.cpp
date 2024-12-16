#include "Field.h"
#include "Engine/Model.h"

Field::Field(GameObject* parent)
	:GameObject(parent,"Field"),hModel_(-1)
{
}

Field::~Field()
{
}

void Field::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Box.fbx");
	assert(hModel_ >= 0);
}

void Field::Update()
{
	transform_.position_ = { 0,-1,0};
}

void Field::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void Field::Release()
{
}
