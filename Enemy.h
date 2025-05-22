#pragma once
#include "Engine/GameObject.h"

//オブジェクトコピー用
class Enemy :public GameObject
{
	enum ENEMYTYPE
	{
		BLUE,
		YELLOW,
		GREEN,
		RED,
		MAX
	};
	enum HP {
		FULL,
		HALF,
		MINI,
		MAX
	};
	enum ANIMATION
	{
		MOVE,
		DAMEGE,
		DEATH,
		MAX
	};
	enum LOD
	{
		HIGH,
		MIDDLE,
		LOW,
		MAX
	};
	int hModel_[ENEMYTYPE::MAX][HP::MAX][ANIMATION::MAX][LOD::MAX];
	

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

	/// <summary>
	/// 与えたダメージ
	/// </summary>
	/// <returns>ダメージ</returns>
	int CausedDamege();
};

