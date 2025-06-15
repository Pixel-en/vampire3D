#include "EnemySpawn.h"
#include "Field.h"
#include "Player.h"
#include "Humanoid.h"
#include "Engine/Model.h"
#include <string>

using std::string;

namespace {
	const int SPAWNAREA{ 10 };
	const int SPAWNAREALIMIT{ 50 };	//スポーンエリアの制限
	const float SPAWNTIME{ 1.0f };
	const int SPAWNLIMIT{ 1 }; //スポーンする敵の数制限
	const int ENEMYTYPE{ 1 }; //敵の種類数
	const int SPAWNHEIGHT{ 4 }; //敵のスポーン高さ
}


EnemySpawn::EnemySpawn(GameObject* parent)
	:GameObject(parent, "EnemySpawn")
{
	number_ = 1;
	timer = SPAWNTIME;
}

EnemySpawn::~EnemySpawn()
{
}

void EnemySpawn::Initialize()
{
	//モデルのロード
	//事前にロードしておくことで途中で読み込んで時間がかからないようにする
	std::string type[ENEMYTYPE] = { "Humanoid" };
	string Level[ELEVEL::END] = { "Blue","Yellow","Green","Red" };
	string hp[HP::HMAX] = { "Full", "Half", "Mini" };
	string anim[ANIMATION::AMAX] = { "Move", "Hit", "Death" };
	string lod[LOD::LMAX] = { "High", "Middle", "Low" };
	for (int g = 0;g < ENEMYTYPE;g++) {
		for (int h = 0;h < ELEVEL::END;h++) {
			for (int i = 0;i < HP::HMAX;i++) {
				for (int j = 0;j < LOD::LMAX;j++) {
					for (int k = 0;k < ANIMATION::AMAX;k++) {
						Model::Load("Assets\\Model\\Character\\Enemy\\" + type[g] + "-" + Level[h] + "-" + hp[i] + "-" + lod[j] + "-" + anim[k] + ".fbx");
					}
				}
			}
		}
	}


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

			//フィールドの生成がされてないときは出現させない
			if (field->GetFieldPosListSize() != FIELDNUM)
				return;

			Enemy* enemy = nullptr;
			int type = rand() % ENEMYTYPE; //敵の種類をランダムに選ぶ
			switch (type)
			{
			case 0: //Humanoid
				enemy = Instantiate<Humanoid>(this);
				break;
			default:
				break;
			}
			if (enemy == nullptr)
				return;
			enemy->Load(ELEVEL::BLUE, number_);
			int count = 0;
			//出現場所を決める
			do
			{
				count++;
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

				enemy->SetPosition(p->GetPosition().x + x, SPAWNHEIGHT, p->GetPosition().z + z);
			} while (enemy->SelfCollision(field));
			Debug::Log(count, true);
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
