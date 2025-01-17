#pragma once
#include "WeaponObject.h"

class cKnife;

//cKnifeを管理する(レベルなど)
class Knife :public WeaponObject
{
	std::vector<cKnife*> List_;

public:
	Knife(GameObject* parent);

	~Knife();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

//実際に動くオブジェクト
class cKnife : public WeaponObject
{
	void Move() override;

	void ResetSub() override;
public:
	cKnife(GameObject* parent);

	~cKnife();

	//初期化
	void Initialize() override;

	//更新
	//void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//当たり判定
	void OnCollision(GameObject* pTarget) override;
};