#include "Field.h"
#include "Engine/Model.h"
#include "Engine/BoxCollider.h"
#include "Player.h"

namespace {
	const XMFLOAT2 FIELDSIZE{ 100,100 };
	const XMINT2 FIELDGRID{ 3,3 };
}

void Field::SpawnField(int num)
{
	XMFLOAT3 temp = DeleteField(num);


	//左下(-100,-100)から生成
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
	//ここで壁のtransformをセットする

}

XMFLOAT3 Field::DeleteField(int num)
{
	XMFLOAT3 temp = fieldPosList_[num];
	fieldPosList_.clear();
	return temp;
}

XMINT2 Field::ObjectPosConvert(XMINT2 _pos)
{
	//-2,-1,0,1,2に変換
	XMINT2 objNum = { _pos.x % FIELDGRID.x ,_pos.y % FIELDGRID.y };

	//x,yの絶対値が2の時は1にして符号を反転する
	//これによりx,yを-1,0,1に変換する
	if (abs(objNum.x) == 2) {
		objNum.x = -objNum.x / abs(objNum.x);
	}
	if (abs(objNum.y) == 2) {
		objNum.y = -objNum.y / abs(objNum.y);
	}
	return objNum;
}

Field::Field(GameObject* parent)
	:GameObject(parent, "Field"), hModel_(-1), currentNum_(-1)
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

	Transform trans;
	trans.position_.y = 100000;

	for (int i = 0; i < FIELDNUM; i++) {
		hWall_[i] = Model::Load("Assets\\Model\\WallObjects\\WallBldg" + std::to_string(i + 1) + ".fbx");
		//HandleCheck(hWall_[i], "壁のモデルがない");
		Model::SetTransform(hWall_[i], trans);
	}

	transform_.position_ = { 0,0,0 };

}

void Field::Update()
{
}

void Field::Draw()
{
	//オブジェクトのタイル配置位置
	XMINT2 objPosNum[FIELDNUM] = { {-1,-1},{0,-1},{1,-1},
									{-1,0},{0,0},{1,0},
									{-1,1},{0,1},{1,1} };

	ClearCollider();
	for (int i = 0; i < fieldPosList_.size(); i++) {
		Transform trans;
		trans.position_ = fieldPosList_[i];
		Model::SetTransform(hModel_, trans);
		Model::Draw(hModel_);

		//特定のタイルに特定のオブジェクトを置くように計算
		XMINT2 objPos = ObjectPosConvert({ (int)fieldPosList_[i].x, (int)fieldPosList_[i].z });
		for (int j = 0; j < FIELDNUM; j++) {
			if (objPosNum[j].x == objPos.x && objPosNum[j].y == objPos.y) {
				Model::SetTransform(hWall_[j], trans);
				Model::Draw(hWall_[j]);
				break;
			}
		}

		//Model::SetTransform(hWall_[i], trans);
		//Model::Draw(hWall_[i]);
		//XMFLOAT3 leftbone = Model::GetBonePosition(hWall_, "WallLeft");
		//BoxCollider* collider = new BoxCollider(leftbone, XMFLOAT3(1, 10, 1));
		//AddCollider(collider);
		//XMFLOAT3 rightbone = Model::GetBonePosition(hWall_, "WallRight");
		//BoxCollider* collider2 = new BoxCollider(rightbone, XMFLOAT3(1, 10, 1));
		//AddCollider(collider2);
	}
	colliderList_;
}

void Field::Release()
{
}



bool Field::RayCastField(XMFLOAT3& _pos, float _rayHeight, std::string _name, float _limit)
{


	for (int j = 0; j < FIELDNUM; j++) {
		if (hWall_[j] == -1)
			continue;
		//レイ
		RayCastData data;
		data.start = _pos;   //レイの発射位置
		data.start.y += _rayHeight;
		data.dir = XMFLOAT3(0, -1, 0);       //レイの方向

		Model::RayCast(hWall_[j], &data); //レイを発射


		//レイが当たったら
		if (data.hit)
		{
			if (data.dist - _rayHeight >= -_limit && data.dist - _rayHeight <= _limit) {
				_pos.y -= data.dist - _rayHeight;
				return true;
				break;
			}
		}
	}

	//地面の判定
	for (int i = 0; i < fieldPosList_.size(); i++) {

		//レイ
		RayCastData data;
		data.start = _pos;   //レイの発射位置
		data.start.y += _rayHeight;
		data.dir = XMFLOAT3(0, -1, 0);       //レイの方向

		//ハンドルにポジションをセットしなおす
		Transform trans;
		trans.position_ = fieldPosList_[i];
		Model::SetTransform(hModel_, trans);
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
