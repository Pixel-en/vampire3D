#pragma once
#include "Engine/GameObject.h"

//敵の強さ
enum ELEVEL {
	BLUE = 0,
	YELLOW = 1,
	GREEN = 2,
	RED = 3,
	END
};
enum HP {
	FULL,
	HALF,
	MINI,
	HMAX
};
enum ANIMATION
{
	MOVE,
	HIT,
	DEATH,
	AMAX
};
enum LOD
{
	HIGH,
	MIDDLE,
	LOW,
	LMAX
};

//オブジェクトコピー用
class Enemy :public GameObject
{
protected:
	//モデル
	int hModel_[HP::HMAX][LOD::LMAX][ANIMATION::AMAX];

private:

public:
	struct Status
	{
		float power_;	//攻撃力
		float speed_;	//スピード
		int hp_;		//体力
		float maxhp_;	//最大体力
		float exp_;		//経験値
		ELEVEL level_;	//敵のレベル
		unsigned int number_;	//識別番号
		float invincibletime_;	//無敵時間
	};

private:

	Status status_;
	Status BaseStatus_;
	Status BoostStatus_;

	//タイマー
	float InvincibleTimer_;	//無敵時間

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
	//モデル
	HP ModelHP_;	//モデルのHP
	LOD ModelLOD_;
	ANIMATION ModelAnim_;
	ANIMATION BeModelAnim_;

	//モデルにアニメーションをセットする
	virtual void SetAnimation() {};

	virtual void StatusUpdate();

	//アニメーションのフレームをゲット
	virtual int GetDeathAnimFrame() const = 0;
	virtual int GetHitFrame() const = 0;

public:

	//ステータスのブースト
	void MulBoostStatePower(float _power) { BoostStatus_.power_ *= _power; };
	void MulBoostStateSpeed(float _speed) { BoostStatus_.speed_ *= _speed; };
	void MulBoostStateMaxHp(float _maxhp) { BoostStatus_.maxhp_ *= _maxhp; };
	void MulBoostStateExp(float _exp) { BoostStatus_.exp_ *= _exp; };

	void MulBoostState(float _power, float _speed, float _maxhp, float _exp)
	{
		MulBoostStatePower(_power);
		MulBoostStateSpeed(_speed);
		MulBoostStateMaxHp(_maxhp);
		MulBoostStateExp(_exp);
	};

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
	void Load(ELEVEL _level, unsigned int _number);

	virtual void SuperUpdate() override;

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
	/// <returns>実数で返されても困るので整数ダメージ</returns>
	int CausedDamege() {
		
		//移動中以外はダメージを与えない
		if (ModelAnim_ != ANIMATION::MOVE)
			return 0;
		return status_.power_;
	};

	/// <summary>
	/// 経験値を取得
	/// </summary>
	/// <returns>実数で返されても困るので整数経験値</returns>
	int GetEnemyEXP() { return status_.exp_; };

	/// <summary>
	/// ダメージを受けた時
	/// </summary>
	/// <param name="_damege">受けたダメージ</param>
	/// <param name="_knock">ノックバック係数</param>
	/// <returns> 判定した場合trueを返す</returns>
	bool HitDamege(int _damege, float _knock);

	/// <summary>
	/// あたり判定で敵のリストを取得
	/// </summary>
	/// <param name="pTarget">当たった敵</param>
	/// <param name="MyItr">自分の当たったコライダーのイテレーター</param>
	/// <param name="list">当たった敵のコライダーリスト</param>
	void OnCollisionsList(GameObject* pTarget, std::list<Collider*>::iterator MyItr, std::list<Collider*> list) override;

};

