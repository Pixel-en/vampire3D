#pragma once
#include "ArmorObject.h"

class MuscleSuit :public ArmorObject
{
public:
	MuscleSuit(GameObject* parent);

	~MuscleSuit();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

