#pragma once
#include "ArmorObject.h"

class FreeMind :public ArmorObject
{
public:
	FreeMind(GameObject* parent);

	~FreeMind();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

