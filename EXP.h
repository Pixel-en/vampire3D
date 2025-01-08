#pragma once
#include "Engine/GameObject.h"

//経験値
class EXP :public GameObject
{
	int hModel_;

public:
	EXP(GameObject* parent);

	~EXP();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

