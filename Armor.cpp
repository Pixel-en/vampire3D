#include "Armor.h"

Armor::Armor(GameObject* parent)
	:ArmorObject(parent,"Armor")
{
}

Armor::~Armor()
{
}

void Armor::Initialize()
{
	Leave();
}

void Armor::Update()
{
}

void Armor::Release()
{
}

void Armor::LevelUp(std::string str)
{
}
