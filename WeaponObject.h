#pragma once
#include "Engine/GameObject.h"
#include "Player.h"
#include "Engine/Model.h"



//武器の継承元
//継承元なだけで実際にインスタンスは作らない
//仮想クラス
class WeaponObject :public GameObject
{
protected:

	int hModel_;

	XMFLOAT3 originPos;	//攻撃開始原点

	struct Status
	{
		int damege_;	//攻撃力
		float speed_;	//移動スピード
		int hp_;	//貫通などに使う
	};

	Status status_;

	//動き方を書く
	virtual void Move() = 0;

public:
	WeaponObject(GameObject* parent);

	WeaponObject(GameObject* parent, const std::string& name);

	~WeaponObject();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

