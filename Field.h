#pragma once
#include "Engine/GameObject.h"

class Field :public GameObject
{
	int hModel_;

public:
	Field(GameObject* parent);

	~Field();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;
};

