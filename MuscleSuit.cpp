#include "MuscleSuit.h"
#include "Player.h"
#include <sstream>

MuscleSuit::MuscleSuit(GameObject* parent)
	: ArmorObject(parent,"MuscleSuit")
{
}

MuscleSuit::~MuscleSuit()
{
}

void MuscleSuit::Initialize()
{
}

void MuscleSuit::Update()
{
}

void MuscleSuit::Release()
{
}

void MuscleSuit::LevelUp(std::string str)
{
	numLevel_++;

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		ATTACK,
		CRITICAL,
	};

	while (std::getline(ss, temp, '/'))
	{
		//’l‚ðŽæ“¾
		float val = std::stof(temp);
		float boost;
		switch (count)
		{
		case ATTACK:
			//‘O‰ñ‚Ì’Ç‰Á•ª‚Ì‹t”‚ð‚©‚¯‚é‚±‚Æ‚Å‘O‰ñ‚Ì•ª‚ðÁ‚·
			boost = val / BeBuff_;
			player->MultDivBoostStatusStrength(boost, true);
			BeBuff_ = val;
			break;
		case CRITICAL:
			boost = val / BeDBuff_;
			player->MultDivBoostStatusCritical(boost, true);
			BeDBuff_ = val;
			break;
		default:
			break;
		}
		count++;
	}
}
