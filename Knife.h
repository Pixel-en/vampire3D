#pragma once
#include "WeaponObject.h"


class Knife :public WeaponObject
{

	void Move() override;
public:
	Knife(GameObject* parent);

	~Knife();

	//‰Šú‰»
	void Initialize() override;

	//XV
	//void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
};

