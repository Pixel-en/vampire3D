#pragma once
#include "Engine/GameObject.h"

namespace {
	//敵の強さ
	enum LEVEL {
		BLUE,
		YELLOW,
		GREEN,
		RED,
		END
	};
}

//オブジェクトコピー用
class Enemy :public GameObject
{
protected:
	enum HP {
		FULL,
		HALF,
		MINI,
		MAX
	};
	enum ANIMATION
	{
		MOVE,
		HIT,
		DEATH,
		MAX
	};
	enum LOD
	{
		HIGH,
		MIDDLE,
		LOW,
		MAX
	};
	//モデル
	int hModel_[HP::MAX][LOD::MAX][ANIMATION::MAX];

private:
	struct Status
	{
		int power_;	//攻撃力
		float speed_;	//スピード
		int hp_;		//体力
		int exp_;		//経験値
		LEVEL level_;	//敵のレベル
		unsigned int number_;	//識別番号
	};
	Status status_;
	
	HP ModelHP_;	//モデルのHP
	LOD ModelLOD_;
	ANIMATION ModelAnim_;

	//動作
	//移動前のポジション
	XMFLOAT3 prePos_;
	//地面についているか
	bool onGround_;
	//フレームごとの重力加速
	float gravity_;

	//動き
	void Move();

protected:
	//モデルにアニメーションをセットする
	virtual void SetAnimation() {};

public:
	Enemy(GameObject* parent);
	Enemy(GameObject* parent, const std::string& name);

	virtual ~Enemy();

	//初期化
	virtual void Initialize() override;

	/// <summary>
	/// データのロード
	/// </summary>
	/// <param name="_level">敵のレベル(enum)</param>
	/// <param name="_number">敵の識別番号</param>
	virtual void Load(LEVEL _level, unsigned int _number);

	//更新
	virtual void Update() override;

	//描画
	virtual void Draw() override;

	//開放
	virtual void Release() override;

	/// <summary>
	/// 敵の識別番号を取得
	/// </summary>
	/// <returns>識別番号</returns>
	unsigned int GetEnemyNumber() { return status_.number_; };

	/// <summary>
	/// 与えたダメージ
	/// </summary>
	/// <returns>ダメージ</returns>
	int CausedDamege() { return status_.power_; };

	/// <summary>
	/// 経験値を取得
	/// </summary>
	/// <returns>経験値</returns>
	int GetEnemyEXP() { return status_.exp_; };

	/// <summary>
	/// ダメージを受けた時
	/// </summary>
	/// <param name="_damege">受けたダメージ</param>
	/// <param name="_knock">ノックバック係数</param>
	void HitDamege(int _damege, float _knock);

	/// <summary>
	/// あたり判定で敵のリストを取得
	/// </summary>
	/// <param name="pTarget">当たった敵</param>
	/// <param name="MyItr">自分の当たったコライダーのイテレーター</param>
	/// <param name="list">当たった敵のコライダーリスト</param>
	void OnCollisionsList(GameObject* pTarget, std::list<Collider*>::iterator MyItr, std::list<Collider*> list) override;

};

