#pragma once
#include "Engine/GameObject.h"

//オブジェクトコピー用
class PlayLoad :public GameObject
{
	//背景用ハンドル
	int hBGImage_;
	//ロード中の画像ハンドル
	int hLDImage_;

	Transform trans;

public:
	PlayLoad(GameObject* parent);

	~PlayLoad();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

