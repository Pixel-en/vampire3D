#pragma once
#include "WeaponObject.h"

//オブジェクトコピー用
class Knife :public WeaponObject
{

	void Move() override;
public:
	Knife(GameObject* parent);

	~Knife();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

