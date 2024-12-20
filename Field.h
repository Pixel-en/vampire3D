#pragma once
#include "Engine/GameObject.h"
#include <vector>

class Field :public GameObject
{
	int hModel_;

	std::vector<XMFLOAT3> fieldPosList_;

	//プレイヤーのいる配列番号
	int currentNum_;
	void SpawnField(int num);
	XMFLOAT3 DeleteField(int num);

public:
	Field(GameObject* parent);

	~Field();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	/// <summary>
	/// 地面との当たり判定
	/// </summary>
	/// <param name="_pos">当たり判定したい側のポジション</param>
	/// <param name="_rayHeight">レイの高さ</param>
	/// <param name="_name">送ってきたオブジェクト</param>
	/// <returns>当たっているかどうか</returns>
	bool RayCastField(XMFLOAT3& _pos, float _rayHeight, std::string _name);

	/// <summary>
	/// 地面との当たり判定
	/// </summary>
	/// <param name="_pos">当たり判定したい側のポジション</param>
	/// <param name="_rayHeight">レイの高さ</param>
	/// <returns>当たっているかどうか</returns>
	bool RayCastField(XMFLOAT3& _pos, float _rayHeight);
};

