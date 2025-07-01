#include "EnemySpawn.h"
#include "Field.h"
#include "Player.h"
#include "Humanoid.h"
#include "Engine/Model.h"
#include <string>
#include "PlayScene.h"

using std::string;

namespace {
	const int SPAWNAREA{ 10 };
	const int SPAWNAREALIMIT{ 50 };	//スポーンエリアの制限
	const float SPAWNTIME{ 1.0f };
	const int SPAWNLIMIT{ 150 }; //スポーンする敵の数制限 200でも動くけど150くらいまでにしたほうがいいかも
	const int ENEMYTYPE{ 1 }; //敵の種類数
	const int SPAWNHEIGHT{ 4 }; //敵のスポーン高さ
	const int INITENEMYNUM{ 10 }; //初期スポーン数
	const int ENEMYLEVEL[ELEVEL::END] = { 120,240,360,480 }; //敵のレベルごとのスポーン数
	const float BOOSTTIME = { 30.0f }; //ブーストの時間

	const float BOOSTPOWER = { 0.05f }; //レベルごとの攻撃力ブースト
	const float BOOSTSPEED = { 0.05f }; //レベルごとのスピードブースト
	const float BOOSTMAXHP = { 0.15f }; //レベルごとの最大体力ブースト
	const float BOOSTEXP = { 0.4f }; //レベルごとの経験値ブースト
}

EnemySpawn::EnemySpawn(GameObject* parent)
	:GameObject(parent, "EnemySpawn")
{
	number_ = 1;
	timer = SPAWNTIME;
	EnemyNum_ = INITENEMYNUM; //初期スポーン数

	BoostStatus_.power_ = 1;	//攻撃力
	BoostStatus_.speed_ = 1;	//スピード
	BoostStatus_.maxhp_ = 1;	//最大体力
	BoostStatus_.exp_ = 1;		//経験値

	BeLevel_ = ELEVEL::END;	//前回のレベル
	BoostCount_ = 0; //ブーストカウント
	boostTimer_ = BOOSTTIME; //ブーストタイマー
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
	for (int g = 0; g < ENEMYTYPE; g++) {
		for (int h = 0; h < ELEVEL::END; h++) {
			for (int i = 0; i < HP::HMAX; i++) {
				for (int j = 0; j < LOD::LMAX; j++) {
					for (int k = 0; k < ANIMATION::AMAX; k++) {
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
		if (EnemyList_.size() < EnemyNum_)
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

			SetEnemyData(enemy);

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

				enemy->SetPosition(p->GetPosition().x + x, SPAWNHEIGHT, p->GetPosition().z + z);
			} while (enemy->SelfCollision(field));
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

void EnemySpawn::AddEnemyNum()
{
	AddEnemyNum(1);
}

void EnemySpawn::AddEnemyNum(int num)
{
	EnemyNum_ += num;
	if (EnemyNum_ > SPAWNLIMIT)
		EnemyNum_ = SPAWNLIMIT;
}

void EnemySpawn::SetEnemyData(Enemy* enemy)
{
	ELEVEL level = ELEVEL::END; //初期レベル
	PlayScene* scene = GetRootJob()->FindGameObject<PlayScene>();
	float timer = scene->GetPlayTimer();

	for (int i = 0; i < ELEVEL::END; i++)
	{
		if (timer < ENEMYLEVEL[i]) {
			level = static_cast<ELEVEL>(i);
			break;
		}
	}

	enemy->Load(level, number_);

	if (timer >= boostTimer_) {
		//前回とレベルが変わったらカウントを0に戻す
		if (level != BeLevel_) {
			BoostCount_ = 0;
		}
		else
			BoostCount_++;

		boostTimer_ += BOOSTTIME; //ブーストタイマーをリセット
	}

	BoostStatus_.power_ = 1 + (BoostCount_ * BOOSTPOWER);	//攻撃力
	BoostStatus_.speed_ = 1 + (BoostCount_ * BOOSTSPEED);	//スピード
	BoostStatus_.maxhp_ = 1 + (BoostCount_ * BOOSTMAXHP);	//最大体力
	BoostStatus_.exp_ = 1 + (BoostCount_ * BOOSTEXP);	//経験値

	enemy->MulBoostState(BoostStatus_.power_, BoostStatus_.speed_, BoostStatus_.maxhp_, BoostStatus_.exp_);

	BeLevel_ = level; //前回のレベルを更新
}