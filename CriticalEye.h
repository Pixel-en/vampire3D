#pragma once
#include "ArmorObject.h"

class CriticalEye :public ArmorObject
{
public:
	CriticalEye(GameObject* parent);

	~CriticalEye();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

