#pragma once
#include "ArmorObject.h"

class Monocle :public ArmorObject
{
	float Buff_;
	float MBuff_;

	bool isMul_;
	bool MisMul_;
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

