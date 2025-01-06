#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//武器の継承元
//継承元なだけで実際にインスタンスは作らない
//仮想クラス
class WeaponObject :public GameObject
{
protected:

	int hModel_;

	XMFLOAT3 originPos;	//攻撃開始原点
	float attackTimer_;
	bool allowsMove_;	//moveを許可するか

	struct Status
	{
		int damege_;	//攻撃力
		float speed_;	//移動スピード
		int hp_;	//貫通などに使う
	};

	Status status_;

	//動き方を書く
	virtual void Move() {};

	//攻撃を出す
	virtual void Reset();

	//攻撃を止める
	//当たった時など
	virtual void Stop();

public:
	WeaponObject(GameObject* parent);

	WeaponObject(GameObject* parent, const std::string& name);

	virtual ~WeaponObject();

	//初期化
	virtual void Initialize() override;

	//更新
	virtual void Update() override;

	//描画
	virtual void Draw() override;

	//開放
	virtual void Release() override;

	virtual void OnCollision(GameObject* pTarget) override;
};

