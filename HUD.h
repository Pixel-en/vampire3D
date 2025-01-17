#pragma once
#include "Engine/GameObject.h"

#include "Engine/Text.h"
#include <vector>
#include <string>
#include <utility>
#include <set>

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

	/*------レベル用------*/

	int hLevelBack_;			//レベルアップの背景用画像
	int hLevelGaugeFrame_;		//経験値のフレーム
	int hLevelGaugeBar_;		//経験値のバー

	bool Pause_;						//ポーズ中かどうか

	Transform LBackTransform_;
	Transform LGFrameTransform_;
	Transform LGBarTransform_;
	void LevelInitialize();
	void LevelSuperUpdate();
	void LevelUpdate();
	void LevelDraw();
	void ObtainWeapon(int _num);

	Text* ptext_;

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

	/// <summary>
	/// レベルが上がった時に行う
	/// </summary>
	void LevelUP();
};

