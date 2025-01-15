#pragma once
#include "WeaponObject.h"


class PoisonThrow :public WeaponObject
{

	int angle_;

	void Move() override;

	//攻撃を出す
	void Reset() override;

public:
	PoisonThrow(GameObject* parent);

	~PoisonThrow();

	//初期化
	void Initialize() override;

	//更新
	//void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
};

