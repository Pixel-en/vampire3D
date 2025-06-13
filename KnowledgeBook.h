#pragma once
#include "ArmorObject.h"

class KnowledgeBook :public ArmorObject
{
public:
	KnowledgeBook(GameObject* parent);

	~KnowledgeBook();

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//ŠJ•ú
	void Release() override;

	void LevelUp(std::string str) override;
};

