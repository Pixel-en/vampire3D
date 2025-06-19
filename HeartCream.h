#pragma once
#include "ArmorObject.h"

class HeartCream :public ArmorObject
{
	float timer_;	//タイマー
	float maxTime_;	//最大時間
	float heal_;	//回復率
	float HpRate_;	//体力割合

public:
	HeartCream(GameObject* parent);

	~HeartCream();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//開放
	void Release() override;

	void LevelUp(std::string str) override;
};

