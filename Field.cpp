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
	hModel_ = Model::Load("Assets\\Model\\Test_Ground.fbx");
	assert(hModel_ >= 0);
	model = Model::Load("Assets\\Model\\Test_Ground.fbx", 1);
	assert(model >= 0);
}

void Field::Update()
{
	transform_.position_ = { 0,-1,0};
	trans = transform_;
	trans.position_.x += 50.0;
}

void Field::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	
	Model::SetTransform(model, trans);
	Model::Draw(model);

}

void Field::Release()
{
}
