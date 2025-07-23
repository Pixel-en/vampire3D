#pragma once
#include "Engine/GameObject.h"

//オブジェクトコピー用
class Effect :public GameObject
{

	void LoadFIle();

public:
	Effect(GameObject* parent);

	~Effect();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	/// <summary>
	/// エフェクトを再生する
	/// </summary>
	/// <param name="ObjName">呼び出し元の名前</param>
	/// <param name="EmName">エミッターの名前</param>
	/// <param name="_pos">ポジション</param>
	/// <returns>エミッターのハンドル</returns>
	int PlayEffect(std::string ObjName,std::string EmName,XMFLOAT3 _pos);
};

