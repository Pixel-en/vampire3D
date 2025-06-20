#include "MagicHand.h"
#include "Player.h"

MagicHand::MagicHand(GameObject* parent)
	:ArmorObject(parent, "MagicHand")
{
}

MagicHand::~MagicHand()
{
}

void MagicHand::Initialize()
{
}

void MagicHand::Update()
{
}

void MagicHand::Release()
{
}

void MagicHand::LevelUp(std::string str)
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	float val = std::stof(str) / BeBuff_;
	player->MultDivBoostStatusCollectionRange(val, true);
}
