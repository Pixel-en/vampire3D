#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"

//武器の継承元
//継承元なだけで実際にインスタンスは作らない
//仮想クラス

//動きの流れ↓
//初期化(init->Move->Stop->Reset->Move->
class WeaponObject :public GameObject
{
protected:

	int hModel_;

	XMFLOAT3 originPos;	//攻撃開始原点
	float ReStartTimer_;
	float AttackTime_;		//攻撃時間
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

	//攻撃のリセット
	void Reset();
	//継承先で追加でリセットしたいことがあるとき用
	virtual void ResetSub() {};

	//攻撃を止める
	//当たった時など
	//継承先で呼ばないと止まらない
	virtual void Stop();

	virtual void Penetration();	//貫通時

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

