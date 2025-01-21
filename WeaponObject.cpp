#include "WeaponObject.h"
#include "Player.h"
#include "sstream"
#include "Engine/CsvReader.h"

namespace {
}

void WeaponObject::Reset()
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_ = player->GetTransform();
	varia_.originPos_ = player->GetPosition();
	varia_.ReStartTimer_ = status_.restart_;

	Visible();
	Clash();
	varia_.allowsMove_ = true;
	varia_.peneCount_ = status_.hp_;

	ResetSub();
}

void WeaponObject::Stop()
{
	Invisible();
	NonClash();
	varia_.allowsMove_ = false;
}

void WeaponObject::Penetration()
{
	varia_.peneCount_--;
	if (varia_.peneCount_ <= 0)
		Stop();
}

void WeaponObject::StatusCSVRead()
{
	CsvReader csv;
	csv.Load("Assets\\CSV\\WeaponInitStatus.csv");

	for (int i = 0; i < csv.GetHeight(); i++) {
		if (csv.GetString(0, i) == objectName_) {
			status_.damege_ = csv.GetValue(1, i);
			status_.speed_ = csv.GetValue(2, i);
			status_.hp_ = csv.GetValue(3, i);
			status_.restart_ = csv.GetValue(4, i);
			status_.Range_ = csv.GetValue(5, i);
			status_.duration_ = csv.GetValue(6, i);
			status_.size_ = csv.GetValue(7, i);
			originStatus_ = status_;
			break;
		}
	}
}

WeaponObject::WeaponObject(GameObject* parent)
	:GameObject(parent, ""), hModel_(-1)
{
	status_.Lv_ = 1;
	status_.damege_ = 0;
	status_.speed_ = 0;
	status_.hp_ = 0;
	status_.restart_ = 0;
	status_.Range_ = 0;
	status_.duration_ = 0;
	status_.size_ = 0;

	varia_.allowsMove_ = true;
	varia_.ReStartTimer_ = status_.restart_;

	StatusCSVRead();
}

WeaponObject::WeaponObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name), hModel_(-1)
{


	status_.Lv_ = 1;
	status_.damege_ = 0;
	status_.speed_ = 0;
	status_.hp_ = 0;
	status_.restart_ = 0;
	status_.Range_ = 0;
	status_.duration_ = 0;
	status_.size_ = 0;

	varia_.allowsMove_ = true;
	varia_.ReStartTimer_ = status_.restart_;

	StatusCSVRead();
}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::Initialize()
{
	Reset();
}

void WeaponObject::Update()
{
	if (varia_.allowsMove_)
		Move();
}

void WeaponObject::Draw()
{
}

void WeaponObject::Release()
{
}

void WeaponObject::OnCollision(GameObject* pTarget)
{
}

void WeaponObject::LevelUp(std::string str)
{
	status_.Lv_++;
	std::stringstream ss{ str };
	std::string temp;

	//–½—ß‚ðˆê‚Â‚¸‚Â‚É•ªŠ„
	while (std::getline(ss, temp, '/'))
	{
		std::stringstream stemp{ temp };
		std::string mini;

		char c;
		float val;
		int count = 0;
		//–½—ß‚ð–½—ß•”‚Æ’l‚É•ÏŠ·
		while (std::getline(stemp, mini, ':'))
		{
			if (count == 0)
				c = *mini.c_str();
			else
				val = std::stof(mini);
			count++;
		}

		switch (c)
		{
		case 'H':
			status_.hp_ += val;
			break;
		case 'D':
			if (val < 1) {
				status_.damege_ += val;
			}
			else {
				status_.damege_ += originStatus_.damege_ * (1.0 + val);
			}
			break;
		case 'S':
			status_.size_ += originStatus_.size_ * (1.0 + val);
			break;
		case 'B':
			for (int i = 0; i < val; i++)
				AddBullet();
			break;
		case 'C':
			status_.restart_ -= val;
			break;
		case 'L':
			status_.Range_ += val;
			break;
		case 'T':
			status_.duration_ += val;
			break;
		case 'E':
			break;
		default:
			break;
		}

	}
}
