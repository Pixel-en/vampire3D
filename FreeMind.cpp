#include "FreeMind.h"
#include "Player.h"

FreeMind::FreeMind(GameObject* parent)
	:ArmorObject(parent, "FreeMind")
{
}

FreeMind::~FreeMind()
{
}

void FreeMind::Initialize()
{
}

void FreeMind::Update()
{
}

void FreeMind::Release()
{
}

void FreeMind::LevelUp(std::string str)
{
	numLevel_++;

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	//プレイヤーのクリティカル倍率の上昇
	float value = std::stof(str);

	player->SetBoostStatusCriticalBoost(value);
}
