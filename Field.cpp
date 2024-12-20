#include "Field.h"
#include "Engine/Model.h"
#include "Player.h"

namespace {
	const XMFLOAT2 FIELDSIZE{ 50,50 };
	const XMFLOAT2 FIELDGRID{ 3,3 };
}

void Field::SpawnField(int num)
{
	XMFLOAT3 temp = DeleteField(num);

	for (int i = 0; i < FIELDGRID.y; i++) {
		for (int j = 0; j < FIELDGRID.x; j++) {
			XMFLOAT3 data = { temp.x + (FIELDSIZE.x * (j - (int)(FIELDGRID.x / 2))),-1,temp.z + (FIELDSIZE.y * (i - (int)(FIELDGRID.y / 2))) };
			fieldPosList_.push_back(data);
		}
	}

	currentNum_ = fieldPosList_.size() / 2;
	Debug::Log(temp);
	Debug::Log("を中心に生成", true);
}

XMFLOAT3 Field::DeleteField(int num)
{
	XMFLOAT3 temp = fieldPosList_[num];
	fieldPosList_.clear();
	return temp;
}

Field::Field(GameObject* parent)
	:GameObject(parent,"Field"),hModel_(-1),currentNum_(-1)
{
}

Field::~Field()
{
}

void Field::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Test_Ground.fbx");
	assert(hModel_ >= 0); 
	fieldPosList_ = { {0,-1,0} };
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
				p->SetonGround(true);
				if (currentNum_ != i)
					SpawnField(i);
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
