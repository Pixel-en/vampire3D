#pragma once
#include "Engine/GameObject.h"
#include <vector>

class HUD :public GameObject
{
	/*------レーダー-----*/
	int hRadar_;	//レーダー用の画像ハンドル
	int hREnemy_;	//レーダー上の敵用画像ハンドル
	int hRPlayer_;	//レーダー上のプレイヤー用画像ハンドル

	Transform RadarTransform_;	//レーダー用トランスフォーム
	Transform REnemyTransform_;	//レーダー上の敵用トランスフォーム
	Transform RPlayerTransform_;//レーダー上のプレイヤー用トランスフォーム
	std::vector<XMFLOAT3> REnemyPosList_;	//レーダー上の敵の位置のリスト
	void RadarInitialize();
	void RadarUpdate();
	void RadarDraw();

	/*------レベルアップ用------*/
	int hLevelBack_;			//レベルアップの背景用画像
	int hLevelGaugeFrame_;
	int hLevelGaugeBar_;

	Transform LBackTransform_;
	Transform LGFrameTransform_;
	Transform LGBarTransform_;
	void LevelInitialize();
	void LevelUpdate();
	void LevelDraw();

public:
	HUD(GameObject* parent);

	~HUD();

	//初期化
	void Initialize() override;

	void SuperUpdate() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

