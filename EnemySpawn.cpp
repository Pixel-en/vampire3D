#include "EnemySpawn.h"
#include "Player.h"

namespace {
	const int SPAWNAREA{ 10 };
	const int SPAWNAREALIMIT{ 50 };
	const float SPAWNTIME{ 1.0f };
	const int SPAWNLIMIT{ 50 };
}


EnemySpawn::EnemySpawn(GameObject* parent)
	:GameObject(parent,"EnemySpawn")
{
	number_ = 1;
	timer = SPAWNTIME;
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
		if (EnemyList_.size() < SPAWNLIMIT)
		{

			Enemy* e = Instantiate<Enemy>(this);
			//oŒ»êŠ‚ðŒˆ‚ß‚é
			int x, z;
			x = (rand() % SPAWNAREA) + SPAWNAREALIMIT;
			z = (rand() % SPAWNAREA) + SPAWNAREALIMIT;
			int signX = rand() % 2;
			int signZ = rand() % 2;

			//•„†‚ð‚Â‚¯‚é
			if (signX == 1)
				x = x * -1;
			if (signZ == 1)
				z = z * -1;

			e->SetPosition(p->GetPosition().x + x, 0, p->GetPosition().z + z);
			e->SetEnemyNumber(number_);
			EnemyList_.push_back(e);
			timer = SPAWNTIME;
			number_++;
		}
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
