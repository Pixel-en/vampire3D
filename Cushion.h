#pragma once
#include "ArmorObject.h"

class Cushion :public ArmorObject
{
	float stopTimer_;
	float stopMaxTime_;
	float BuffTimer_;
	float BuffMaxTime_;
	float Buff_;

	enum STATE {
		ATTACK,
		SPEED,
		RANGE,
		HASTE,
		CRITICAL,
		MAX
	};

	STATE upState_;

public:
	Cushion(GameObject* parent);

	~Cushion();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

