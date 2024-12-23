#pragma once
#include "Engine/GameObject.h"

//敵個人
class Enemy :public GameObject
{

	int hModel_;

public:
	Enemy(GameObject* parent);

	~Enemy();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	int GetModelHandle() { return hModel_; }
};

