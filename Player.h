#pragma once
#include "Engine/GameObject.h"

class Player :public GameObject
{
	int hmodel;
	int hImage_;

	Transform cursorTrans_;

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

