#pragma once
#include "ArmorObject.h"

class WonderCookie :public ArmorObject
{
	float BeBuf_;


public:
	WonderCookie(GameObject* parent);

	~WonderCookie();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

