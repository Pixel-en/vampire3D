#pragma once
#include "Engine/GameObject.h"

//敵個人
class Enemy :public GameObject
{
	enum ANIMATION {
		MOVE,
		DEATH,
		MAX,
	};

	ANIMATION anim_;

	int hModel_[ANIMATION::MAX];
	int hModelLow_;

	unsigned int number_;

	struct Status
	{
		int power_;		//攻撃力
		float speed_;	//スピード
		int hp_;		//体力
		int exp_;
	};

	Status status_;

	float InvincibleTimer_;	//無敵時間

	/// <summary>
	/// 動き
	/// </summary>
	void Move();

	//前にポーズをしていたか
	bool prevPause_;
	bool isLow_;

public:
	Enemy(GameObject* parent);

	~Enemy();

	//初期化
	void Initialize() override;

	void SuperUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	/// <summary>
	/// 敵の番号をセット
	/// </summary>
	/// <param name="_number">番号</param>
	void SetEnemyNumber(unsigned int _number) { number_ = _number; };

	/// <summary>
	/// 敵の番号を取得
	/// </summary>
	/// <returns>敵の番号</returns>
	unsigned int GetEnemyNumber() { return number_; };

	int GetEnemyEXP() { return status_.exp_; };

	/// <summary>
	/// ダメージを受けた時
	/// </summary>
	/// <param name="_damege">受けたダメージ</param>
	/// <param name="_knock">ノックバック係数</param>
	void HitDamege(int _damege,float _knock);

	/// <summary>
	/// ダメージを与えた
	/// </summary>
	/// <returns>ダメージ</returns>
	int CausedDamege();

};

