#include "CriticalEye.h"
#include "Player.h"

CriticalEye::CriticalEye(GameObject* parent)
	:ArmorObject(parent, "CriticalEye")
{
}

CriticalEye::~CriticalEye()
{
}

void CriticalEye::Initialize()
{
}

void CriticalEye::Update()
{
}

void CriticalEye::Release()
{
}

void CriticalEye::LevelUp(std::string str)
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	//プレイヤーのクリティカル倍率の上昇
	float value = std::stof(str);

	player->SetStatusCriticalBoost(player->GetBaseStatus().criticalBoost_*value);
}
