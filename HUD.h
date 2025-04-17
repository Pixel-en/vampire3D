#pragma once
#include "Engine/GameObject.h"

#include <vector>
#include <string>
#include <set>

class HUD :public GameObject
{
	/*------レーダー-----*/
	int hRadarFrame_;	//レーダーのフレーム用画像ハンドル
	int hRadarBack_;	//レーダーの背景用画像ハンドル
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
	int hLevelCursor_;
	int hLevelFrameImage_;
	int hLevelCursorImage_;

	bool Pause_;						//ポーズ中かどうか

	struct WeaponLevel
	{
		std::string name_;						//武器の名前
		int num_;								//武器の番号
		int MaxLevel_;							//最大レベル
		std::vector<std::string> instruction_;	//レベルアップ内容
		std::string EffectText_[10];				//効果説明
	};

	std::vector<WeaponLevel> WeaponList_;	//出現していないorレベルが最大ではない武器のリスト
	std::set<int> RollListNum_;				//ロールされた武器
	int levelCursor_;
	bool StickTriggerY_;					//前フレームでスティックを倒しているか

	void WeaponRoll();	//武器のロール

	Transform LBackTransform_;
	Transform LGFrameTransform_;
	Transform LGBarTransform_;
	Transform LCursorTransform_;
	Transform LFrameTransform_;
	void LevelInitialize();
	void LevelSuperUpdate();
	void LevelUpdate();
	void LevelDraw();
	void ObtainWeapon(int _num);

	/*-----タイマー-----*/
	float PlayTime_;
	void TimerDraw();

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

	void SetTimer(float _time) { PlayTime_ = _time; }
};

