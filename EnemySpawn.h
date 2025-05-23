#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Enemy.h"

//敵を出現させる
class EnemySpawn :public GameObject
{
	//敵の識別番号
	unsigned int number_;
	//敵のスポーンタイマー
	float timer;
	//敵のリスト
	std::vector<Enemy*> EnemyList_;

public:
	EnemySpawn(GameObject* parent);

	~EnemySpawn();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	/// <summary>
	/// 敵のリストを取得
	/// </summary>
	/// <returns>敵のリストを返す</returns>
	std::vector<Enemy*> GetEnemyList();
};

