#pragma once
#include "Engine/GameObject.h"

//プレイヤー
class Player :public GameObject
{
	bool onGround_;	//地面についているか
	float gravity;	//重力
	float speed_;	//スピード

	int hModel_;			//モデルハンドル
	float lookHeight_;	//視点の高さ

	int hImage_;	//画像のハンドル(クロスヘア)
	Transform crossTrans;	//クロスヘア用のトランス


	//キャラクターの移動
	void Move();
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
};

