#include "Missile.h"
#include "EnemySpawn.h"
#include <algorithm>

void Missile::AddBullet()
{
	Missile::cMissile* c = Instantiate<Missile::cMissile>(GetParent());
	c->SetStatus(status_);
	//止まっているなら残りの時間+バッファ
	if (!List_[0]->isMove()) {
		c->SetResetTimer(List_[0]->GetResetTimer() + (List_.size() * BUFFER));
	}
	//動いているなら残りの攻撃持続時間+リセットタイム+バッファ
	else {
		c->SetResetTimer(List_[0]->GetResetTimer() + List_[0]->GetAttackTimer() + (List_.size() * BUFFER));
	}
	List_.push_back(c);
}

Missile::Missile(GameObject* parent)
	:WeaponObject(parent,"Missile")
{
	Missile::cMissile* c = Instantiate<Missile::cMissile>(GetParent());
	List_.push_back(c);
}

Missile::~Missile()
{
}

void Missile::Initialize()
{
}

void Missile::Update()
{
	for (int i = 0; i < List_.size(); i++) {
		if (List_[i]->GetLv() != status_.Lv_)
			List_[i]->SetNextStatus(status_);
	}
}

void Missile::Draw()
{
}

void Missile::Release()
{
}

//-------------------------------

void Missile::cMissile::Move()
{
	XMVECTOR pFront = { 0,0,1,0 };
	XMVECTOR dir;

	if (UpTimer_ < 0.0f) {
		
		if (varia_.AttackTime_ < 0.0f) {
			Stop();
			return;
		}
		else {
			varia_.AttackTime_ -= Time::DeltaTime();
		}

		if (!Search_) {
			transform_.rotate_.x = 0;
			targetpos_ = searchEnemy();
			Search_ = true;
			varia_.originPos_ = transform_.position_;

			XMVECTOR pos = XMLoadFloat3(&transform_.position_);
			XMVECTOR tar = XMLoadFloat3(&targetpos_);

			XMVECTOR distance = XMVector3Normalize(tar - pos);

			float dot = XMVectorGetX(XMVector3Dot(pFront, distance));
			dot = std::clamp(dot, -1.0f, 1.0f);
			float angle = acos(dot);

			XMVECTOR cross = XMVector3Cross(distance, pFront);
			cross = XMVector3Normalize(cross);

			if (XMVectorGetY(cross) >= 0) {
				transform_.rotate_.y = -XMConvertToDegrees(angle);
			}
			else {
				transform_.rotate_.y = XMConvertToDegrees(angle);
			}
			
			Debug::Log("Missile");
			Debug::Log(transform_.rotate_.y, true);
		}
		XMVECTOR origin = XMLoadFloat3(&varia_.originPos_);
		XMVECTOR target = XMLoadFloat3(&targetpos_);
		dir = target - origin;
		dir = XMVector3Normalize(dir);
		transform_.rotate_.z += 5;
	}
	else {
		UpTimer_ -= Time::DeltaTime();
		transform_.rotate_.x = -90;
		XMMATRIX rot = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
		dir = XMVector3Normalize(XMVector3TransformCoord(pFront, rot));
		Search_ = false;
	}

	transform_.position_ += dir * status_.speed_ * Time::DeltaTime();
}

void Missile::cMissile::ResetSub()
{
	UpTimer_ = 0.5f;
	transform_.rotate_.z = 0;
}

void Missile::cMissile::Penetration()
{
	if (varia_.peneCount_ == -1)
		return;

	varia_.peneCount_--;
	if (varia_.peneCount_ <= 0)
		Stop();
}

XMFLOAT3 Missile::cMissile::searchEnemy()
{
	EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
	std::vector<Enemy*> List = ep->GetEnemyList();

	float distance = fabs(varia_.originPos_ - List[0]->GetPosition());
	int num = 0;
	for (int i = 1; i < List.size(); i++) {
		float subdis = fabs(varia_.originPos_ - List[i]->GetPosition());
		if (distance > subdis) {
			distance = subdis;
			num = i;
		}
	}
	return List[num]->GetPosition();
}

Missile::cMissile::cMissile(GameObject* parent)
	:WeaponObject(parent, "cMissile")
{
}

Missile::cMissile::~cMissile()
{
}

void Missile::cMissile::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Missile.fbx");
	if (hModel_ < 0)
		MessageBox(NULL, "モデルが見つかりません", objectName_.c_str(), MB_OK);
}

void Missile::cMissile::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Missile::cMissile::Release()
{
}
