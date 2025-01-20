#pragma once
#include "Engine/GameObject.h"

//オブジェクトコピー用
class a:public GameObject
{
public:
	a(GameObject* parent);

	~a();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

