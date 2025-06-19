#pragma once
#include "EnergyDrink.h"

EnergyDrink::EnergyDrink(GameObject* parent)
	:ArmorObject(parent, "EnergyDrink")
{
}

EnergyDrink::~EnergyDrink()
{
}

void EnergyDrink::Initialize()
{
	Enter();
}

void EnergyDrink::Update()
{
}

void EnergyDrink::Release()
{
}

void EnergyDrink::LevelUp(std::string str)
{
}
