#pragma once
#include "Engine/GameObject.h"
#include "WeaponObject.h"
#include <vector>

//プレイヤー
class Player :public GameObject
{
	std::vector<WeaponObject*> weaponList_;

	struct Status
	{
		int hp_;				//体力
		float speed_;			//移動速度
		float critical_;		//クリティカル
		float collectionRange_;	//回収範囲
		float haste_;			//攻撃頻度

		//内部データ
		int resist_;			//防御

	};

	Status status_;

	bool onGround_;	//地面についているか
	float gravity;	//重力

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

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;
};

