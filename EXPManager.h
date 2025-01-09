#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "EXP.h"

//経験値管理クラス
//インスタンスを持たない仮想クラス
class EXPManager :public GameObject
{
	std::vector<EXP> EXPList_;

	int EXPStorage_;	//出現させれなかった経験値をためておく

public:
	EXPManager(GameObject* parent);

	~EXPManager();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	
	/// <summary>
	/// 経験値を出現させる
	/// </summary>
	/// <param name="pos">出現場所</param>
	/// <param name="_exp">経験値</param>
	void SpawnEXP(XMFLOAT3 pos, int _exp);
};

