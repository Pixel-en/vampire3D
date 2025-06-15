#include "WideAmulet.h"
#include "Player.h"

WideAmulet::WideAmulet(GameObject* parent)
	:ArmorObject(parent, "WideAmulet")
{
}

WideAmulet::~WideAmulet()
{
}

void WideAmulet::Initialize()
{
}

void WideAmulet::Update()
{
}

void WideAmulet::Release()
{
}

void WideAmulet::LevelUp(std::string str)
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	//プレイヤーのクリティカル倍率の上昇
	float value = std::stof(str);

	player->SetStatusArea(player->GetBaseStatus().area_ * value);

	for (auto itr = player->MyWeaponList_.begin(); itr != player->MyWeaponList_.end(); itr++)
	{
		(*itr)->CollisionSizeSet();
	}
}
