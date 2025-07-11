#include "LifeFragment.h"
#include "Player.h"
#include <sstream>

LifeFragment::LifeFragment(GameObject* parent)
	:ArmorObject(parent, "LifeFragment")
{
	BeHP_ = 0.0f;
	BuffSpeed_ = 1.0f;
	rate_ = 0.0f;
	BuffFlag_ = false;	
}

LifeFragment::~LifeFragment()
{
}

void LifeFragment::Initialize()
{
	Enter();
}

void LifeFragment::Update()
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	if (player->GetStatus().hp_ <= player->GetStatus().maxHp_ * rate_) {
		if (!BuffFlag_) {
			player->MultDivBoostStatusSpeed(BuffSpeed_, true);
			BuffFlag_ = true;
		}
	}
	else {
		if (BuffFlag_) {
			player->MultDivBoostStatusSpeed(BuffSpeed_, false);
			BuffFlag_ = false;
		}
	}

}

void LifeFragment::Release()
{
}

void LifeFragment::LevelUp(std::string str)
{
	numLevel_++;

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	std::stringstream ss(str);
	std::string temp;
	int count = 0;
	enum {
		HP,
		RATE,
		SPPED
	};

	while (std::getline(ss,temp,'/'))
	{
		float val = std::stof(temp);
		switch (count)
		{
		case HP:
			player->AddStatusMaxHp(val - BeHP_);
			player->StatusUpdate();	//一度ステータスの更新をすることで最大体力を増やす
			player->HealingHp(val - BeHP_); //HPも回復する
			BeHP_ = val;
			break;
		case RATE:
			rate_ = val;
			break;
		case SPPED:
			if (BuffFlag_) //バフがかかっているなら
			player->MultDivBoostStatusSpeed(BuffSpeed_, false);
			BuffSpeed_ = val;
			BuffFlag_ = false; //初期化
			break;
		default:
			break;
		}
		count++;
	}
}
