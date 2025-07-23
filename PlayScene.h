#pragma once
#include "Engine/GameObject.h"

class PlayScene : public GameObject
{
	float PlayTimer_;

	int hIntroSound_;
	int hLoopSound_;

	//ロードしたか
	bool isLoaded_;

	//オブジェクトのロード
	void LoadObject();
	int LoadCount_;

	bool isClear_;
	float clearTimer_;

	bool isGameOver_;
	float gameOverTimer_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	void SuperUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	float GetPlayTimer() { return PlayTimer_; }

	void PlayerDead();
};