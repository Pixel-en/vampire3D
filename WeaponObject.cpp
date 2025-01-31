#include "WeaponObject.h"
#include "Player.h"
#include "sstream"
#include "EnemySpawn.h"


void WeaponObject::Reset()
{
	ResetBefore();

	//ステータスの更新と当たり判定のサイズの変更
	if (status_.Lv_ < nextStatus_.Lv_) {
		status_ = nextStatus_;

		for (auto itr = colliderList_.begin(); itr != colliderList_.end();itr++) {
			(*itr)->ChengeSize(status_.size_);
		}
	}


	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_ = player->GetTransform();
	varia_.originPos_ = player->GetPosition();
	varia_.ReStartTimer_ = status_.restart_;

	Visible();
	Clash();
	varia_.allowsMove_ = true;
	varia_.peneCount_ = status_.hp_;
	varia_.AttackTime_ = status_.duration_;

	ResetSub();
}

void WeaponObject::Stop()
{
	ReStartWait();
	varia_.allowsMove_ = false;
}

void WeaponObject::ReStartWait()
{
	Invisible();
	NonClash();
}

void WeaponObject::Penetration()
{
	if (varia_.peneCount_ == -1)
		return;

	varia_.peneCount_--;
	if (varia_.peneCount_ <= 0)
		ReStartWait();
}

void WeaponObject::StatusInitGet()
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	if (player->WeaponStateWrite(objectName_, status_)) {
		originStatus_ = status_;
	}
}

WeaponObject::WeaponObject(GameObject* parent)
	:GameObject(parent, ""), hModel_(-1)
{

	targetName_ = "";

	status_.Lv_ = -1;
	status_.damege_ = 0;
	status_.speed_ = 0;
	status_.hp_ = 0;
	status_.restart_ = 0;
	status_.Range_ = 0;
	status_.duration_ = 0;
	status_.size_ = 0;
	
	varia_.ReStartTimer_ = status_.restart_;

	StatusInitGet();
	nextStatus_ = status_;

	Reset();
	varia_.allowsMove_ = false;
}

WeaponObject::WeaponObject(GameObject* parent, const std::string& name)
	:GameObject(parent, name), hModel_(-1)
{
	targetName_ = "";

	status_.Lv_ = -1;
	status_.damege_ = 0;
	status_.speed_ = 0;
	status_.hp_ = 0;
	status_.restart_ = 0;
	status_.Range_ = 0;
	status_.duration_ = 0;
	status_.size_ = 0;

	varia_.ReStartTimer_ = status_.restart_;

	StatusInitGet();
	nextStatus_ = status_;

	Reset();
	varia_.allowsMove_ = false;
}

WeaponObject::~WeaponObject()
{
}

void WeaponObject::Initialize()
{
	varia_.allowsMove_ = false;
}

void WeaponObject::Update()
{
	if (varia_.allowsMove_) {
		Move();
	}
	//一定時間経過したらリセット
	else {
		if (varia_.ReStartTimer_ < 0.0f) {
			Reset();
		}
		else {
			varia_.ReStartTimer_ -= Time::DeltaTime();
		}
	}
}

void WeaponObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void WeaponObject::Release()
{
}

void WeaponObject::OnCollision(GameObject* pTarget)
{

	if (pTarget->GetObjectName() == "Enemy")
	{
		EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
		std::vector<Enemy*> List = ep->GetEnemyList();
		for (int i = 0; i < List.size(); i++) {
			if (dynamic_cast<Enemy*>(pTarget)->GetEnemyNumber() == List[i]->GetEnemyNumber()) {
				List[i]->HitDamege(status_.damege_, status_.knockback_);
				Penetration();
				break;
			}
		}
	}
}

void WeaponObject::LevelUp(std::string str)
{
	status_.Lv_++;
	std::stringstream ss{ str };
	std::string temp;

	//命令を一つずつに分割
	while (std::getline(ss, temp, '/'))
	{
		std::stringstream stemp{ temp };
		std::string mini;

		char c;
		float val = 0;
		int count = 0;
		//命令を命令部と値に変換
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
