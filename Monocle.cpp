#include "Monocle.h"
#include "Player.h"
#include <sstream>

Monocle::Monocle(GameObject* parent)
	:ArmorObject(parent, "Monocle")
{
	Buff_ = 1.0f;
	MBuff_ = 1.0f;
	isMul_ = false;
	MisMul_ = false;
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

	//プレイヤーの体力が最大なら
	if (player->GetStatus().maxHp_ == player->GetStatus().hp_) {
		if (!MisMul_) {
			//パッシブを解除してマックス時の付ける
			//バフがついているなら消す
			if (isMul_)
				player->MultDivBoostStatusCritical(Buff_, false);

			player->MultDivBoostStatusCritical(MBuff_, true);
			MisMul_ = true;
			isMul_ = false;
		}

	}
	else {
		if (!isMul_) {
			//バフがついているなら消す
			if (MisMul_)
				player->MultDivBoostStatusCritical(MBuff_, false);
			
			player->MultDivBoostStatusCritical(Buff_, true);
			isMul_ = true;
			MisMul_ = false;
		}
	}
}

void Monocle::Release()
{
}

void Monocle::LevelUp(std::string str)
{
	numLevel_++;

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	//前のバフを消す
	if (player->GetStatus().maxHp_ == player->GetStatus().hp_)
		player->MultDivBoostStatusCritical(MBuff_, false);
	else
		player->MultDivBoostStatusCritical(Buff_, true);

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		CRITICAL,
		MAXCRITICAL
	};

	while (std::getline(ss, temp, '/'))
	{
		//値を取得
		float val = std::stof(temp);
		switch (count)
		{
		case CRITICAL:
			Buff_ = val;
			break;
		case MAXCRITICAL:
			MBuff_ = val;
			break;
		default:
			break;
		}
		count++;
	}

	//付け直すためリセット
	isMul_ = false;
	MisMul_ = false;

}
