#pragma once
#include "ArmorObject.h"

/// <summary>
/// 防御を1上げる　最大レベルは3
/// </summary>
class Armor :public ArmorObject
{
public:
	Armor(GameObject* parent);

	~Armor();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//開放
	void Release() override;

	void LevelUp() override;
};

