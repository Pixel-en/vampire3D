#pragma once
#include "Enemy.h"
class Humanoid:public Enemy
{

	void SetAnimation() override;
	int GetDeathAnimFrame() const override;
	int GetHitFrame() const override;
public:

	Humanoid(GameObject* parent);

	~Humanoid();

	//ŠJ•ú
	void Release() override;
};

