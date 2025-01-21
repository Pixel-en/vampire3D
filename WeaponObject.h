#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include <vector>

//武器の継承元
//継承元なだけで実際にインスタンスは作らない
//仮想クラス

//動きの流れ↓
//初期化(init->Move->Stop->Reset->Move->
class WeaponObject :public GameObject
{
	struct WeaponVariables
	{
		XMFLOAT3 originPos_;	//攻撃開始原点
		float ReStartTimer_;
		float AttackTime_;		//攻撃時間
		bool allowsMove_;	//moveを許可するか
		int peneCount_;		//貫通回数
	};

	struct Status
	{
		int Lv_;			//武器のレベル
		int damege_;		//攻撃力
		float speed_;		//移動スピード
		int hp_;			//貫通などに使う
		float restart_;		//リスタートまでの時間を減らす
		int Range_;			//距離
		float duration_;	//持続時間
		float size_;		//当たり判定のサイズ
	};

public:
	WeaponVariables varia_;	//弾個々の変数

protected:

	int hModel_;

	Status status_;		//全体共有のステータス
	Status originStatus_;	//ステータスの初期値
	
	//動き方を書く
	virtual void Move() {};

	//継承先で追加でリセットしたいことがあるとき用
	virtual void ResetSub() {};

	//攻撃を止める
	//当たった時など
	//継承先で呼ばないと止まらない
	virtual void Stop();

	virtual void Penetration();	//貫通時

	virtual void AddBullet() = 0;

	virtual void StatusCSVRead();

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

	//攻撃のリセット
	void Reset();

	virtual void LevelUp(std::string str);

	int GetLv() { return status_.Lv_; }
	void SetStatus(Status _st) { status_ = _st; };
	Status GetStatus() { return status_; }
	bool isMove() { return varia_.allowsMove_; }
};

