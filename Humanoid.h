#pragma once
#include "Enemy.h"
class Humanoid:public Enemy
{

	void SetAnimation() override;

public:

	Humanoid(GameObject* parent);

	~Humanoid();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;
};

