#include "Field.h"
#include "Engine/Model.h"
#include "Player.h"

namespace {
	const XMFLOAT2 FIELDSIZE{ 50,50 };
	const XMINT2 FIELDGRID{ 3,3 };
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
	//ここ修正したい
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

bool Field::RayCastField(XMFLOAT3& _pos, float _rayHeight, std::string _name, float _limit)
{
	for (int i = 0; i < fieldPosList_.size(); i++) {

		//レイ
		RayCastData data;
		data.start = _pos;   //レイの発射位置
		data.start.y += _rayHeight;
		data.dir = XMFLOAT3(0, -1, 0);       //レイの方向

		//ハンドルにポジションをセットしなおす
		transform_.position_ = fieldPosList_[i];
		Model::SetTransform(hModel_, transform_);
		Model::RayCast(hModel_, &data); //レイを発射


		//レイが当たったら
		if (data.hit)
		{
			if (data.dist - _rayHeight >= -_limit && data.dist - _rayHeight <= _limit) {
				_pos.y -= data.dist - _rayHeight;
				if (_name == "Player") {
					if (currentNum_ != i)
						SpawnField(i);
				}
				return true;
				break;
			}
		}
	}
	return false;
}

bool Field::RayCastField(XMFLOAT3& _pos, float _rayHeight, std::string _name)
{
	return RayCastField(_pos, _rayHeight, _name, 1.0f);
}

bool Field::RayCastField(XMFLOAT3& _pos, float _rayHeight, float _limit)
{
	return RayCastField(_pos, _rayHeight, "", _limit);
}

bool Field::RayCastField(XMFLOAT3& _pos, float _rayHeight)
{
	return RayCastField(_pos, _rayHeight, "", 1.0f);

}
