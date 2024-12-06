#pragma once
#include "Engine/GameObject.h"

class Player :public GameObject
{
	int hmodel;
public:
	Player(GameObject* parent);

	~Player();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;
};

