#pragma once
#include "Engine/GameObject.h"
#include "WeaponObject.h"
#include <vector>

//プレイヤー
class Player :public GameObject
{
public:
	std::vector<WeaponObject*> MyWeaponList_;

private:
	struct Status
	{
		int hp_;				//体力
		float speed_;			//移動速度
		float critical_;		//クリティカル
		float collectionRange_;	//回収範囲
		float haste_;			//攻撃頻度

		//内部データ
		int resist_;					//防御
		unsigned int level_;			//現在のレベル
		unsigned int currentExp_;		//今の経験値量
		unsigned int nextLvExp_;		//次のレベルまでの必要経験値量
		unsigned long long totalExp_;	//取得した経験値量
	};

	Status status_;

public:
	unsigned int GetLevel() { return status_.level_; };
	unsigned int GetCurrentExp() { return status_.currentExp_; };
	unsigned int GetnextLvExp() { return status_.nextLvExp_; };
private:

	bool onGround_;	//地面についているか
	float gravity;	//重力
	bool PauseON_;

	int hModel_;			//モデルハンドル
	float lookHeight_;	//視点の高さ
	int hImage_;	//画像のハンドル(クロスヘア)
	Transform crossTrans;	//クロスヘア用のトランス


	//キャラクターの移動
	void Move();

	//プレイヤーの攻撃
	void Attack();

public:
	Player(GameObject* parent);

	~Player();

	//初期化
	void Initialize() override;

	void SuperUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 経験値を取得
	/// </summary>
	/// <param name="_exp">取得した経験値</param>
	void AcquisitionEXP(int _exp);
};

