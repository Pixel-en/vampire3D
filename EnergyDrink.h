#pragma once
#include "ArmorObject.h"

class EnergyDrink :public ArmorObject
{
public:
	EnergyDrink(GameObject* parent);

	~EnergyDrink();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

