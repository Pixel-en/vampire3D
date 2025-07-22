#pragma once
#include "Engine/GameObject.h"

//‰ñ•œ
class Apple :public GameObject
{
	int hModel_;
	float speed_;

	bool dead_;	//æ“¾‚³‚ê‚½

public:
	Apple(GameObject* parent);

	~Apple();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
};

