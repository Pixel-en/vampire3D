#include "HeartCream.h"
#include "Player.h"
#include <sstream>

HeartCream::HeartCream(GameObject* parent)
	:ArmorObject(parent, "HeartCream")
{
	timer_ = 0.0f; // タイマー初期化
	heal_ = 1.0f; // 回復率初期化
}

HeartCream::~HeartCream()
{
}

void HeartCream::Initialize()
{
	Enter();
}

void HeartCream::Update()
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	//プレイヤーの体力が最大値の一定割合以下なら回復
	if (player->GetStatus().maxHp_ * HpRate_ > player->GetStatus().hp_) {
		if (timer_ <= 0.0f) {
			//回復処理
			player->HealingHp(player->GetStatus().maxHp_ * heal_);
			timer_ = maxTime_; // タイマーをリセット
		}
		else {
			timer_ -= Time::DeltaTime(); // タイマーを減少
		}
	}

}

void HeartCream::Release()
{
}

void HeartCream::LevelUp(std::string str)
{
	numLevel_++;

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		HPRATE,
		TIME,
		HEAL
	};

	//分割
	while (std::getline(ss,temp,'/'))
	{
		switch (count)
		{
		case HPRATE:
			HpRate_ = std::stof(temp);
			break;
		case TIME:
			maxTime_ = std::stof(temp);
			break;
		case HEAL:
			heal_ = std::stof(temp);
			break;
		default:
			break;
		}
		count++;
	}
}
