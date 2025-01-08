#include "EXPManager.h"

EXPManager::EXPManager(GameObject* parent)
	:GameObject(parent, "EXPManager")
{
	EXPStorage_ = 0;
}

EXPManager::~EXPManager()
{
}

void EXPManager::Initialize()
{
}

void EXPManager::Update()
{
}

void EXPManager::Draw()
{
}

void EXPManager::Release()
{
}

void EXPManager::SpawnEXP(XMFLOAT3 pos)
{
	if (EXPList_.size() > 100)
		return;

}
