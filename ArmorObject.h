#pragma once
#include "Engine/GameObject.h"

class ArmorObject :public GameObject
{
	int numLevel_;

protected:
	//前回のバフ、デバフの値
	float BeBuff_;
	float BeDBuff_;

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
	//効果内容や数値はそれぞれ異なるので都度実装する
	virtual void LevelUp(std::string str) = 0;

	//現在のレベルを取得
	int GetNumLevel() const { return numLevel_; }
};

