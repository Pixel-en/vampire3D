#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"
#include <vector>

//オブジェクトコピー用
class Effect :public GameObject
{

	void LoadFile();


	struct EmitterFile
	{
		std::string objectName;	//オブジェクト名
		std::string emitterName;	//エミッター名
		EmitterData emitterData;	//エミッターのデータ
	};

	std::vector<EmitterFile> emitterList_;	//エミッターのデータを格納するリスト

public:
	Effect(GameObject* parent);

	~Effect();

	//初期化
	void Initialize() override;

	void Update() override {};

	void Draw() override {};

	//開放
	void Release() override;

	/// <summary>
	/// エフェクトを再生する
	/// </summary>
	/// <param name="ObjName">呼び出し元の名前</param>
	/// <param name="EmName">エミッターの名前</param>
	/// <param name="_pos">ポジション</param>
	/// <returns>エミッターのハンドル</returns>
	int PlayEffect(std::string ObjName, std::string EmName, XMFLOAT3 _pos);
};

