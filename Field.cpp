#include "Field.h"
#include "Engine/Model.h"
#include "Player.h"

namespace {
	const float RAYHEIGHT{ 5.0f };
}

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
	fieldPosList_ = { {-50,-1, 50}, {0,-1, 50}, {50,-1, 50}
					, {-50,-1,  0}, {0,-1,  0}, {50,-1,  0}
					, {-50,-1,-50}, {0,-1,-50}, {50,-1,-50} };
	//fieldPosList_ = { {0,-1,0} };
}

void Field::Update()
{
	//Player* p = GetParent()->FindGameObject<Player>();

	////レイ
	//RayCastData data;
	//data.start = p->GetPosition();		//レイの発射位置
	////data.start.y += RAYHEIGHT;
	//data.dir = XMFLOAT3(0, -1, 0);			//レイの方向
	//p->SetonGround(false);
	//Debug::Log("start");
	//Debug::Log(data.start.y, true);
	//for (int i = 0; i < fieldPosList_.size(); i++) {
	//	transform_.position_ = fieldPosList_[i];
	//	transform_.Calclation();
	//	Model::SetTransform(hModel_, transform_);
	//	Model::RayCast(hModel_, &data);

	//	if (data.hit) {
	//		p->SetonGround(true);
	//		dist_ = data.dist - RAYHEIGHT;
	//		break;
	//	}
	//	else
	//		dist_ = 0;
	//}

	//Debug::Log(data.hit, true);
	//Debug::Log(p->GetPosition().y, true);

	//Debug::Log(data.dist, true);
}

void Field::Draw()
{
	for (int i = 0; i < fieldPosList_.size(); i++) {
		transform_.position_ = fieldPosList_[i];
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	}

}

void Field::Release()
{
}
