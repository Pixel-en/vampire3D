#include "Field.h"
#include "Engine/Model.h"
#include "Player.h"

namespace {
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
	//fieldPosList_ = { {0,-1,0},{50,-1,  0} };
}

void Field::Update()
{
	Player* p = GetParent()->FindGameObject<Player>();


	
	for (int i = 0; i < fieldPosList_.size();i++) {

		//レイ
		RayCastData data;
		data.start = p->GetRayStart();   //レイの発射位置
		data.dir = XMFLOAT3(0, -1, 0);       //レイの方向

		//ハンドルにポジションをセットしなおす
		transform_.position_ = fieldPosList_[i];
		Model::SetTransform(hModel_, transform_);
		Model::RayCast(hModel_, &data); //レイを発射


		p->SetonGround(false);
		dist_ = 0;
		//レイが当たったら
		if (data.hit)
		{
			if (data.dist - p->GetRayHeight() >= -1.0f && data.dist - p->GetRayHeight() <= 1.0f) {
				dist_ = data.dist - p->GetRayHeight();
				Debug::Log(dist_, true);
				p->SetonGround(true);
				break;
			}
		}
	}
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
