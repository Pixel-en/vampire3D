#pragma once
#include "Enemy.h"
class Humanoid:public Enemy
{
	enum ENEMYTYPE
	{
		BLUE,
		YELLOW,
		GREEN,
		RED,
		MAX
	};
	enum HP {
		FULL,
		HALF,
		MINI,
		MAX
	};
	enum ANIMATION
	{
		MOVE,
		DAMEGE,
		DEATH,
		MAX
	};
	enum LOD
	{
		HIGH,
		MIDDLE,
		LOW,
		MAX
	};
	int hModel_[ENEMYTYPE::MAX][HP::MAX][ANIMATION::MAX][LOD::MAX];

public:

	Humanoid(GameObject* parent);

	~Humanoid();

	//èâä˙âª
	void Initialize() override;

	//çXêV
	void Update() override;

	//ï`âÊ
	void Draw() override;

	//äJï˙
	void Release() override;
};

