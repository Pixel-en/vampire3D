#include "Humanoid.h"
#include "Engine/Model.h"

namespace {
	const int MOVEANIMFRAME{ 23 };
	const int DEATHANIMFRAME{ 56 } ;
	const int HITANIMFRAME{ 19 };
}

void Humanoid::SetAnimation()
{
	for (int i = 0;i < HP::HMAX;i++) {
		for (int j = 0;j < LOD::LMAX;j++) {
			for (int k = 0;k < ANIMATION::AMAX;i + k++) {

				switch (k)
				{
				case Enemy::MOVE:
					Model::SetAnimFrame(hModel_[i][j][k], 0, MOVEANIMFRAME * 2, 1.0f);
					break;
				case Enemy::HIT:
					Model::SetAnimFrame(hModel_[i][j][k], 0, HITANIMFRAME * 2, 1.0f);
					break;
				case Enemy::DEATH:
					Model::SetAnimFrame(hModel_[i][j][k], 0, DEATHANIMFRAME * 2, 1.0f);
					break;
				default:
					break;
				}
			}
		}
	}
}

Humanoid::Humanoid(GameObject* parent)
	:Enemy(parent,"Humanoid")
{
}

Humanoid::~Humanoid()
{
}

void Humanoid::Initialize()
{
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
