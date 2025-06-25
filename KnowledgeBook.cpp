#include "KnowledgeBook.h"
#include "Player.h"

KnowledgeBook::KnowledgeBook(GameObject* parent)
	:ArmorObject(parent, "KnowledgeBook")
{
}

KnowledgeBook::~KnowledgeBook()
{
}

void KnowledgeBook::Initialize()
{
}

void KnowledgeBook::Update()
{
}

void KnowledgeBook::Release()
{
}

void KnowledgeBook::LevelUp(std::string str)
{
	numLevel_++;

	Player* player = GetRootJob()->FindGameObject<Player>();
	//プレイヤーなかったらどうしようね
	if (player == nullptr)
		return;
	//プレイヤーの経験値に加算する
	float value = std::stof(str);

	player->SetBoostStatusExpBoost(value);
}
