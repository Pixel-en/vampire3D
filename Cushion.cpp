#include "Cushion.h"
#include "Player.h"
#include <sstream>

Cushion::Cushion(GameObject* parent)
	:ArmorObject(parent, "Cushion")
{
	stopTimer_ = 0.0f;
	stopMaxTime_ = 0.0f;
	BuffTimer_ = 0.0f;
	BuffMaxTime_ = 0.0f;
	Buff_ = 1.0f;
	upState_ = MAX;
}

Cushion::~Cushion()
{
}

void Cushion::Initialize()
{
	Enter();
}

void Cushion::Update()
{

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	//動いていないなら
	if (!player->isMove()) {
		//カウントして時間になったかつバフが切れているなら
		if (BuffTimer_ <= 0.0f) {
			if (stopTimer_ <= 0.0f) {
				stopTimer_ = stopMaxTime_;
				BuffTimer_ = BuffMaxTime_;
				//バフをつける
				int up = rand() % STATE::MAX;

				switch (up)
				{
				case STATE::ATTACK:
					player->MultDivBoostStatusStrength(Buff_, true);
					upState_ = STATE::ATTACK;
					break;
				case STATE::SPEED:
					player->MultDivBoostStatusSpeed(Buff_, true);
					upState_ = STATE::SPEED;
					break;
				case STATE::RANGE:
					player->MultDivBoostStatusCollectionRange(Buff_, true);
					upState_ = STATE::RANGE;
					break;
				case STATE::HASTE:
					player->MultDivBoostStatusHaste(Buff_, true);
					upState_ = STATE::HASTE;
					break;
				case STATE::CRITICAL:
					player->MultDivBoostStatusCritical(Buff_, true);
					upState_ = STATE::CRITICAL;
					break;
				default:
					break;
				}

			}
			else {
				stopTimer_ -= Time::DeltaTime();
			}
		}
	}
	else {
		stopTimer_ = stopMaxTime_;
	}

	if (BuffTimer_ <= 0.0f)
	{

		//バフを外す
		//一度だけしなければならない
		switch (upState_)
		{
		case Cushion::ATTACK:
			player->MultDivBoostStatusStrength(Buff_, false);
			upState_ = Cushion::MAX; //リセット
			break;
		case Cushion::SPEED:
			player->MultDivBoostStatusSpeed(Buff_, false);
			upState_ = Cushion::MAX; //リセット
			break;
		case Cushion::RANGE:
			player->MultDivBoostStatusCollectionRange(Buff_, false);
			upState_ = Cushion::MAX; //リセット
			break;
		case Cushion::HASTE:
			player->MultDivBoostStatusHaste(Buff_, false);
			upState_ = Cushion::MAX; //リセット
			break;
		case Cushion::CRITICAL:
			player->MultDivBoostStatusCritical(Buff_, false);
			upState_ = Cushion::MAX; //リセット
			break;
		case Cushion::MAX:
			//何もしない
			break;
		default:
			break;
		}
	}
	else {
		BuffTimer_ -= Time::DeltaTime();
	}
	
}

void Cushion::Release()
{
}

void Cushion::LevelUp(std::string str)
{

	numLevel_++;

	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		STOPTIME,
		BUFFTIME,
		BUFF,
	};

	while (std::getline(ss,temp,'/'))
	{
		//値を取得
		float val = std::stof(temp);
		switch (count)
		{
		case STOPTIME:
			stopMaxTime_ = val;
			if (stopTimer_ <= 0.0f)
				stopTimer_ = stopMaxTime_;
			break;
		case BUFFTIME:
			BuffMaxTime_ = val;
			break;
		case BUFF:
			//一度バフを外す
			switch (upState_)
			{
			case Cushion::ATTACK:
				player->MultDivBoostStatusStrength(Buff_, false);
				break;
			case Cushion::SPEED:
				player->MultDivBoostStatusSpeed(Buff_, false);
				break;
			case Cushion::RANGE:
				player->MultDivBoostStatusCollectionRange(Buff_, false);
				break;
			case Cushion::HASTE:
				player->MultDivBoostStatusHaste(Buff_, false);
				break;
			case Cushion::CRITICAL:
				player->MultDivBoostStatusCritical(Buff_, false);
				break;
			case Cushion::MAX:
				//何もしない
				break;
			default:
				break;
			}
			Buff_ = val;

			switch (upState_)
			{
			case STATE::ATTACK:
				player->MultDivBoostStatusStrength(Buff_, true);
				break;
			case STATE::SPEED:
				player->MultDivBoostStatusSpeed(Buff_, true);
				break;
			case STATE::RANGE:
				player->MultDivBoostStatusCollectionRange(Buff_, true);
				break;
			case STATE::HASTE:
				player->MultDivBoostStatusHaste(Buff_, true);
				break;
			case STATE::CRITICAL:
				player->MultDivBoostStatusCritical(Buff_, true);
				break;
			default:
				break;
			}

			break;
		default:
			break;
		}
		count++;
	}
}
