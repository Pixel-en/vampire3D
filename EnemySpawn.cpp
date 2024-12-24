#include "EnemySpawn.h"
#include <vector>
#include "Enemy.h"
#include "Player.h"

namespace {
	std::vector<Enemy*> EnemyList_;
	float timer = 1.0f;
}


EnemySpawn::EnemySpawn(GameObject* parent)
	:GameObject(parent,"EnemySpawn")
{
}

EnemySpawn::~EnemySpawn()
{
}

void EnemySpawn::Initialize()
{
	Leave();
	Instantiate<Enemy>(this);
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
			x = (rand() % 40) - 20;
			z = (rand() % 40) - 20;
			if ((x > -10 && x < 10) || (z > -10 && z < 10))
				continue;
			break;
		}
		e->SetPosition(p->GetPosition().x + x, 0, p->GetPosition().z + z);
		EnemyList_.push_back(e);
		timer = 1.0f;
	}
	else
		timer -= Time::DeltaTime();
}

void EnemySpawn::Draw()
{
}

void EnemySpawn::Release()
{
}
