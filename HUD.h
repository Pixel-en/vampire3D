#pragma once
#include "Engine/GameObject.h"

#include <vector>
#include <string>
#include <set>

namespace{
	const int LEVELMAX{ 10 };
	enum WEAPONTYPE {
		KNIFE,
		POISONTHROW,
		SPIKEORB,
		MISSILE,
		LASER,
		BOMB,
		END,

		//防具は101から
		ARMOR=101,
		FREEMIND,
		WIDEAMULET,
		KNOWLEDGEBOOK,
		HEARTCREAM,
		WONDERCOOKIE,
		MUSCLESUIT,
		ENERGYDRINK,
		MAGICHAND,
		MONOCLE,
		CUSHION,
		LIFEFRAGMENT,
		AEND,

		MAXHP=201,
		SPD,
		STR,
		CRT,
		COLLECT,
		HASTE,
		SEND
	};
}

class HUD :public GameObject
{


private:
	enum TRANSFORMTYPE
	{
		//レーダー
		RADAR,
		RADARENEMY,
		RADARPLAYER,
		//レベル
		LEVELBACK,
		LEVELGAUGEFRAME,
		LEVELGAUGEBAR,
		LEVELCURSOR,
		LEVELFRAME,
		LEVELICON,
		//HP
		HPBACK,
		HPGAUGE,
		HPFRAME,
		HPICON,
		//装備
		EQUIPICON,
		EQUIPBACK,
		EQUIPMENTS,


		MAX
	};

	Transform HUDTransforms_[TRANSFORMTYPE::MAX];	//UIのTransform

	/*------レーダー-----*/
	int hRadarFrame_;	//レーダーのフレーム用画像ハンドル
	int hRadarBack_;	//レーダーの背景用画像ハンドル
	int hREnemy_;	//レーダー上の敵用画像ハンドル
	int hRPlayer_;	//レーダー上のプレイヤー用画像ハンドル

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
	int hLevelIconImage_[WEAPONTYPE::END + (WEAPONTYPE::AEND - WEAPONTYPE::ARMOR) + (WEAPONTYPE::SEND - WEAPONTYPE::MAXHP)];	//武器のアイコン

	bool Pause_;						//ポーズ中かどうか

	struct EquipmentLevel
	{
		std::string name_;						//装備の名前
		std::string displayName_;				//装備の表示名
		int num_;								//装備の番号
		int MaxLevel_;							//最大レベル
		std::vector<std::string> instruction_;	//レベルアップ内容
		std::string EffectText_[LEVELMAX];				//効果説明
	};

	std::vector<EquipmentLevel> EquipmentList_;	//出現していないorレベルが最大ではない武器のリスト
	std::vector<EquipmentLevel> StatusUpList_;	//ステータスアップのリスト
	std::set<int> RollListNum_;				//ロールされた武器
	int levelCursor_;
	bool StickTriggerY_;					//前フレームでスティックを倒しているか

	void WeaponRoll();	//武器のロール

	void LevelInitialize();
	void LevelSuperUpdate();
	void LevelUpdate();
	void LevelDraw();
	void ObtainWeapon(int _num);

	/*-----タイマー-----*/
	float PlayTime_;
	void TimerDraw();
	XMFLOAT2 TimerPos_;

	/*--------HP--------*/
	int hHPBack_;	//HPの背景用画像
	int hHPGauge_;	//HPのゲージ用画像
	int hHPFrame_;	//HPのフレーム用画像
	int hHPFull_;
	int hHPIcon_;
	void HPInitialize();
	void HPUpdate();
	void HPDraw();

	/*--------装備---------*/

	int hEquiIcon_;
	int hEquiBack_;
	void EquipmentInitialize();
	void EquipmentDraw();


	void UIPosRead();
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

