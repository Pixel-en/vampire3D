#include "Humanoid.h"
#include "Engine/Model.h"

using std::string;

Humanoid::Humanoid(GameObject* parent)
	:Enemy(parent,"Humanoid")
{
}

Humanoid::~Humanoid()
{
}

void Humanoid::Initialize()
{
	//モデルのロード
	string color[ENEMYTYPE::MAX] = { "Blue", "Yellow", "Green", "Red" };
	string hp[HP::MAX] = { "Full", "Half", "Mini" };
	string animation[ANIMATION::MAX] = { "Run", "Hit", "Death" };
	string lod[LOD::MAX] = { "", "Middle", "Low" };
	for (int i = 0; i < ENEMYTYPE::MAX; i++) {
		for (int j = 0; j < HP::MAX; j++) {
			for (int k = 0; k < ANIMATION::MAX; k++) {
				for (int n = 0; n < LOD::MAX; n++) {
					hModel_[i][j][k][n] = Model::Load("Assets\\Model\\Character\\Enemy\\Enemy" + '-' + color[i] + '-' + hp[j] + '-' + lod[n] + '-' + animation[k] + ".fbx");
					HandleCheck(hModel_[i][j][k][n], color[i] + ',' + hp[j] + ',' + lod[n] + ',' + animation[k] + "の敵のモデルがない");
				}
			}
		}
	}
}

void Humanoid::Update()
{
}

void Humanoid::Draw()
{
}

void Humanoid::Release()
{
}
