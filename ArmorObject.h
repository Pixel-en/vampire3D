#pragma once
#include "Engine/GameObject.h"

class ArmorObject :public GameObject
{
public:
	ArmorObject(GameObject* parent);

	ArmorObject(GameObject* parent, const std::string& name);

	~ArmorObject();

	//初期化
	virtual void Initialize() override;

	//更新
	virtual void Update() override;

	//描画
	virtual void Draw() override;

	//開放
	virtual void Release() override;

	//レベルアップ時
	virtual void LevelUp() {};
};

