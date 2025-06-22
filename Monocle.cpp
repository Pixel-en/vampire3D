#include "Monocle.h"
#include "Player.h"
#include <sstream>

Monocle::Monocle(GameObject* parent)
	:ArmorObject(parent, "Monocle")
{
}

Monocle::~Monocle()
{
}

void Monocle::Initialize()
{
	Enter();
}

void Monocle::Update()
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	if (player->GetStatus().maxHp_ == player->GetStatus().hp_) {
		if (!isMul_) {
			player->MultDivBoostStatusCritical(Buff_, true);
			isMul_ = true;
		}

	}
	else {
		if (isMul_) {
			player->MultDivBoostStatusCritical(Buff_, false);
			isMul_ = false;
		}
	}
}

void Monocle::Release()
{
}

void Monocle::LevelUp(std::string str)
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		CRITICAL,
		MAXCRITICAL
	};

	while (std::getline(ss, temp, '/'))
	{
		//’l‚ðŽæ“¾
		float val = std::stof(temp);

		switch (count)
		{
		case CRITICAL:
			//‘O‰ñ‚Ì’Ç‰Á•ª‚Ì‹t”‚ð‚©‚¯‚é‚±‚Æ‚Å‘O‰ñ‚Ì•ª‚ðÁ‚·
			float boost = val / BeBuff_;
			player->MultDivBoostStatusSpeed(boost, true);
			BeBuff_ = val;
			break;
		case MAXCRITICAL:
			Buff_ = val;
			if (isMul_) {
				player->MultDivBoostStatusCritical(BeBuff2_, false);
			}
			BeBuff2_ = val;
			break;
		default:
			break;
		}
		count++;
	}

}
