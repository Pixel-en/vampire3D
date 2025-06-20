#pragma once
#include "ArmorObject.h"

class Monocle :public ArmorObject
{
	float BeBuff2_;
	float Buff_;
public:
	Monocle(GameObject* parent);

	~Monocle();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

