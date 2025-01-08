#include "EnemySpawn.h"
#include "Player.h"

namespace {
	float timer = 1.0f;

	int SPAWNAREA{ 40 };
}


EnemySpawn::EnemySpawn(GameObject* parent)
	:GameObject(parent,"EnemySpawn")
{
	number_ = 1;
}

EnemySpawn::~EnemySpawn()
{
}

void EnemySpawn::Initialize()
{
}

void EnemySpawn::Update()
{
	Player* p = GetParent()->FindGameObject<Player>();
	if (p == nullptr)
		return;

	if (timer <= 0.0) {

		Enemy* e = Instantiate<Enemy>(this);
		int x, z;
		while (true) {
			x = (rand() % SPAWNAREA) - SPAWNAREA / 2;
			z = (rand() % SPAWNAREA) - SPAWNAREA / 2;
			if ((x > -10 && x < 10) || (z > -10 && z < 10))
				continue;
			break;
		}
		e->SetPosition(p->GetPosition().x + x, 0, p->GetPosition().z + z);
		e->SetEnemyNumber(number_);
		EnemyList_.push_back(e);
		timer = 1.0f;
		number_++;
	}
	else
		timer -= Time::DeltaTime();

	//ƒŠƒXƒg‚©‚çíœ
	for (auto I = EnemyList_.begin(); I != EnemyList_.end();) {
		if ((*I)->IsDead()) {
			I = EnemyList_.erase(I);
		}
		else
			I++;
	}
}

void EnemySpawn::Draw()
{
}

void EnemySpawn::Release()
{
}

std::vector<Enemy*> EnemySpawn::GetEnemyList()
{
	return EnemyList_;
}
