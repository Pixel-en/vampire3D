#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Enemy.h"

//敵を出現させる
class EnemySpawn :public GameObject
{
	unsigned int number_;

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

	std::vector<Enemy*> GetEnemyList();
};

