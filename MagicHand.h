#pragma once
#include "ArmorObject.h"

class MagicHand :public ArmorObject
{
public:
	MagicHand(GameObject* parent);

	~MagicHand();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

