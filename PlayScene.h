#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
#include <vector>

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

	struct EmitterFile
	{
		std::string objectName;	//オブジェクト名
		std::string emitterName;	//エミッター名
		EmitterData emitterData;	//エミッターのデータ
	};

	std::vector<EmitterFile> emitterList_;	//エミッターのデータを格納するリスト

	void EmitterLoad();

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

	void PlayerDead() ;

	//エミッターのデータを返すよ
	EmitterData GetEmitterData(std::string _objectName,std::string EmitterName);
};