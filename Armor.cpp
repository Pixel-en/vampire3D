#include "Armor.h"
#include "Player.h"

Armor::Armor(GameObject* parent)
	:ArmorObject(parent,"Armor")
{
}

Armor::~Armor()
{
}

void Armor::Initialize()
{
}

void Armor::Update()
{
}

void Armor::Release()
{
}

void Armor::LevelUp(std::string str)
{

	numLevel_++;


	Player* player = GetRootJob()->FindGameObject<Player>();
	//プレイヤーなかったらどうしようね
	if (player == nullptr)
		return;
	//プレイヤーの防御力に加算する
	int value = std::stoi(str);

	player->SetBoostStatusResist(value);
}
