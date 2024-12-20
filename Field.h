#pragma once
#include "Engine/GameObject.h"
#include <vector>

class Field :public GameObject
{
	int hModel_;

	std::vector<XMFLOAT3> fieldPosList_;

	float dist_;

	//現在の
	int currentNum_;
	void SpawnField(int num);
	XMFLOAT3 DeleteField(int num);

public:
	Field(GameObject* parent);

	~Field();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//レイの距離を返す
	float GetRayDist() { return dist_; };
};

