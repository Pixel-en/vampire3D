#pragma once
#include "EnergyDrink.h"
#include "Player.h"
#include <sstream>

EnergyDrink::EnergyDrink(GameObject* parent)
	:ArmorObject(parent, "EnergyDrink")
{
	timer_ = 0.0f; // タイマー初期化
	MaxTime_ = 1.0f; // 最大時間初期化
	DBuff_ = 1.0; // デバフ率初期化
}

EnergyDrink::~EnergyDrink()
{
}

void EnergyDrink::Initialize()
{
	Enter();
}

void EnergyDrink::Update()
{

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	if (player->GetStatus().hp_ > 1) {
		if (timer_ <= 0.0f) {
			//HPを減らしたいのでマイナスをつける
			player->HealingHp(player->GetStatus().maxHp_ * DBuff_ * -1);
			timer_ = MaxTime_; // タイマーをリセット
		}
		else {
			timer_ -= Time::DeltaTime(); // タイマーを減少
		}
	}
	else {
		timer_ = MaxTime_; // タイマーをリセット
	}

}

void EnergyDrink::Release()
{
}

void EnergyDrink::LevelUp(std::string str)
{
	numLevel_++;

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		TIME,
		HP,
		ATTACK
	};
	
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;


	while (std::getline(ss,temp,'/'))
	{
		//値を取得
		float val = std::stof(temp);
		float boost;
		switch (count)
		{
		case TIME:
			MaxTime_ = val;
			timer_ = MaxTime_; // タイマーをリセット
			break;
		case HP:
			DBuff_ = val;
			break;
		case ATTACK:
			
			//攻撃と頻度のブーストを行う
			boost = val / BeBuff_;
			player->MultDivBoostStatusStrength(val, true);
			player->MultDivBoostStatusHaste(val, true);
			BeBuff_ = val;
			break;
		default:
			break;
		}
		count++;
	}
}
