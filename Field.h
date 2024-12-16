#pragma once
#include "Engine/GameObject.h"

class Field :public GameObject
{
	int hModel_;

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

	//モデルのハンドルを返す
	int GetModelHandle() { return hModel_; }
};

