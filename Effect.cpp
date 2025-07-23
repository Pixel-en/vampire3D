#include "Effect.h"
#include "Engine/CsvReader.h"

void Effect::LoadFIle()
{
}

Effect::Effect(GameObject* parent)
	:GameObject(parent, "Effect")
{
}

Effect::~Effect()
{
}

void Effect::Initialize()
{
}

void Effect::Update()
{
}

void Effect::Draw()
{
}

void Effect::Release()
{
}

int Effect::PlayEffect(std::string ObjName, std::string EmName, XMFLOAT3 _pos)
{
    return 0;
}
