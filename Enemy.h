#pragma once
#include "Engine/GameObject.h"

//敵個人
class Enemy :public GameObject
{

	int hModel_;

	unsigned int number_;

	struct Status
	{
		int power_;		//攻撃力
		float speed_;	//スピード
		int hp_;		//体力
	};

	Status status_;

	void Move();

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

	void SetEnemyNumber(unsigned int _number) { number_ = _number; };

	unsigned int GetEnemyNumber() { return number_; };

};

