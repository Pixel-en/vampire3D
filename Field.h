#pragma once
#include "Engine/GameObject.h"
#include <vector>

namespace {
	const int FIELDNUM{ 9 };	//地面を出現させる数
}

//地面
class Field :public GameObject
{
	int hModel_;

	int hWall_[FIELDNUM];

	std::vector<XMFLOAT3> fieldPosList_;	//地面のリスト

	//プレイヤーのいる地面の配列番号
	int currentNum_;

	//地面を出現させる
	void SpawnField(int num);
	//地面を消す
	XMFLOAT3 DeleteField(int num);

	//座標からオブジェクトのタイルポジションに計算
	XMINT2 ObjectPosConvert(XMINT2 _pos);

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
	/// <param name="_limit">レイの制限</param>
	/// <returns>当たっているかどうか</returns>
	bool RayCastField(XMFLOAT3& _pos, float _rayHeight, std::string _name, float _limit);

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
	/// <param name="_limit">レイの制限</param>
	/// <returns>当たっているかどうか</returns>
	bool RayCastField(XMFLOAT3& _pos, float _rayHeight, float _limit);

	/// <summary>
	/// 地面との当たり判定
	/// </summary>
	/// <param name="_pos">当たり判定したい側のポジション</param>
	/// <param name="_rayHeight">レイの高さ</param>
	/// <returns>当たっているかどうか</returns>
	bool RayCastField(XMFLOAT3& _pos, float _rayHeight);


	int GetFieldPosListSize() const { return fieldPosList_.size(); }
};

