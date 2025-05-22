#pragma once
#include "Engine/GameObject.h"

//オブジェクトコピー用
class Enemy :public GameObject
{

public:
	Enemy(GameObject* parent);
	Enemy(GameObject* parent, const std::string& name);

	virtual ~Enemy();

	//初期化
	virtual void Initialize() override;

	//更新
	virtual void Update() override;

	//描画
	virtual void Draw() override;

	//開放
	virtual void Release() override;

	/// <summary>
	/// 与えたダメージ
	/// </summary>
	/// <returns>ダメージ</returns>
	int CausedDamege();
};

