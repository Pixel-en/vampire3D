#pragma once
#include "ArmorObject.h"

class WideAmulet :public ArmorObject
{
public:
	WideAmulet(GameObject* parent);

	~WideAmulet();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

