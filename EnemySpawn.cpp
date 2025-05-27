#include "EnemySpawn.h"
#include "Field.h"
#include "Player.h"
#include "Humanoid.h"

namespace {
	const int SPAWNAREA{ 10 };
	const int SPAWNAREALIMIT{ 50 };
	const float SPAWNTIME{ 1.0f };
	const int SPAWNLIMIT{ 1 };
	const int ENEMYTYPE{ 1 }; //敵の種類数
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
			Field* field = GetRootJob()->FindGameObject<Field>();
			Enemy* enemy;
			int type = rand() % ENEMYTYPE; //敵の種類をランダムに選ぶ
			switch (type)
			{
			case 0: //Humanoid
				enemy = Instantiate<Humanoid>(this);
				break;
			default:
				break;
			}
			//出現場所を決める
			do
			{

			int x, z;
			x = (rand() % SPAWNAREA) + SPAWNAREALIMIT;
			z = (rand() % SPAWNAREA) + SPAWNAREALIMIT;
			int signX = rand() % 2;
			int signZ = rand() % 2;

			//符号をつける
			if (signX == 1)
				x = x * -1;
			if (signZ == 1)
				z = z * -1;

			enemy->SetPosition(p->GetPosition().x + x, 0, p->GetPosition().z + z);
			} while (enemy->SelfCollision(field));

			enemy->Load(ELEVEL::BLUE, number_);

			EnemyList_.push_back(enemy);
			timer = SPAWNTIME;
			number_++;
		}
	}
	else
		timer -= Time::DeltaTime();

	//リストから削除
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
