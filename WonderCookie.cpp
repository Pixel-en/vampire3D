#include "WonderCookie.h"
#include "Player.h"
#include <sstream>

WonderCookie::WonderCookie(GameObject* parent)
	:ArmorObject(parent, "WonderCookie")
{

}

WonderCookie::~WonderCookie()
{
}

void WonderCookie::Initialize()
{
}

void WonderCookie::Update()
{
}

void WonderCookie::Release()
{
}

void WonderCookie::LevelUp(std::string str)
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	std::stringstream ss{ str };
	std::string temp;
	int count = 0;
	enum {
		SPEED,
		ATTACK
	};

	while (std::getline(ss,temp,'/'))
	{
		//’l‚ðŽæ“¾
		float val = std::stof(temp);

		switch (count)
		{
		case SPEED:
			//‘O‰ñ‚Ì’Ç‰Á•ª‚Ì‹t”‚ð‚©‚¯‚é‚±‚Æ‚Å‘O‰ñ‚Ì•ª‚ðÁ‚·
			float boost = val / BeBuff_;
			player->MultDivBoostStatusSpeed(boost,true);
			BeBuff_ = val;
			break;
		case ATTACK:
			float boost = val / BeDBuff_;
			player->MultDivBoostStatusStrength(val, true);
			BeDBuff_ = val;
			break;
		default:
			break;
		}
		count++;
	}


}
