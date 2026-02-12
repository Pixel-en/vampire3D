#pragma once
#include "Engine/GameObject.h"

//オブジェクトコピー用
class Test:public GameObject
{
public:
	Test(GameObject* parent);

	~Test();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

