#include "Bomb.h"
#include "EnemySpawn.h"
#include "Player.h"
#include "Effect.h"

namespace {
	const int LISTMAX{ 30 };
	const float DETONATETIME{ 1.5f };
}

void Bomb::AddBullet()
{
	spawncount_++;
}

Bomb::Bomb(GameObject* parent)
	:WeaponObject(parent, "Bomb")
{
	spawncount_ = 1;
}

Bomb::~Bomb()
{
}

void Bomb::Initialize()
{
}

void Bomb::Update()
{

	if (varia_.ReStartTimer_ < 0.0f) {
		//リストの格納数を超えていたら古いのから削除
		if (List_.size() + spawncount_ > LISTMAX) {
			int count = List_.size() + spawncount_ - LISTMAX;
			for (int i = 0; i < count; i++) {
				List_.erase(List_.begin());
			}
		}
		//爆弾を出す
		int rot = 360 / spawncount_;
		for (int i = 0; i < spawncount_; i++) {
			Bomb::cBomb* c = Instantiate<Bomb::cBomb>(GetParent());
			c->SetStatus(status_);
			c->SetRotateY(i * rot);
			List_.push_back(c);
		}
		Reset();
	}
	else {
		//時間計測
		varia_.ReStartTimer_ -= Time::DeltaTime();
	}

	//リセット状態なら消す
	for (auto itr = List_.begin(); itr != List_.end();) {
		if ((*itr)->IsDead())
			itr = List_.erase(itr);
		else {
			itr++;
		}
	}
}

void Bomb::Draw()
{
}

void Bomb::Release()
{
}


void Bomb::cBomb::Move()
{
	if (ignite_) {
		if (detonateTimer_ < 0.0f) {
			detonate_ = true;
		}
		else {
			detonateTimer_ -= Time::DeltaTime();
		}
	}

	if (detonate_) {
		Clash();
		CollisionSizeSet(status_.size_ * 2.0f);

		if (varia_.AttackTime_ < 0.0f) {
			EffectUpdate();
			Stop();
			KillMe();
			return;
		}
		else {
			varia_.AttackTime_ -= Time::DeltaTime();
		}
	}
}

void Bomb::cBomb::ResetSub()
{
	XMVECTOR Front = { 0,0,1,0 };

	XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMVECTOR dir = XMVector3TransformCoord(Front, rot);
	dir = XMVector3Normalize(dir);

	transform_.position_ += dir * 2.0f;
}

void Bomb::cBomb::EffectUpdate()
{
	Effect* effect = GetRootJob()->FindGameObject<Effect>();
	//最悪エフェクトがなくても動くように
	if (effect == nullptr) {
		return;
	}

	effect->PlayEffect("Bomb", "fire", transform_.position_);
	effect->PlayEffect("Bomb", "sparks", transform_.position_);
	effect->PlayEffect("Bomb", "flash", transform_.position_);
}

Bomb::cBomb::cBomb(GameObject* parent)
	:WeaponObject(parent, "cBomb")
{
	detonate_ = false;
	ignite_ = false;
}

Bomb::cBomb::~cBomb()
{
}

void Bomb::cBomb::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Weapon\\Bomb.fbx");
	HandleCheck(hModel_);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1, 0), status_.size_);
	AddCollider(collision);
}

void Bomb::cBomb::Update()
{
	CollisionSizeSet();
	Move();
}

void Bomb::cBomb::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bomb::cBomb::Release()
{
}

void Bomb::cBomb::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectTag() == "Enemy")
	{
		if (!ignite_) {
			ignite_ = true;
			detonateTimer_ = DETONATETIME;
			NonClash();
		}

		if (detonate_) {
			EnemySpawn* ep = GetRootJob()->FindGameObject<EnemySpawn>();
			std::vector<Enemy*> List = ep->GetEnemyList();
			for (int i = 0; i < List.size(); i++) {
				if (dynamic_cast<Enemy*>(pTarget)->GetEnemyNumber() == List[i]->GetEnemyNumber()) {
					List[i]->HitDamege(status_.damege_, status_.knockback_);
					Penetration();
					KillMe();
					break;
				}
			}
		}
	}
}
