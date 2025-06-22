#pragma once
#include "ArmorObject.h"

class LifeFragment :public ArmorObject
{
	float BeHP_;
	float BuffSpeed_;
	float rate_;
	float BuffFlag_;

public:
	LifeFragment(GameObject* parent);

	~LifeFragment();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

