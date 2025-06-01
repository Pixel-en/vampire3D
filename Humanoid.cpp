#include "Humanoid.h"
#include "Engine/Model.h"

namespace {
	const int MOVEANIMFRAME{ 43 };
	const int DEATHANIMFRAME{ 111 };
	const int HITANIMFRAME{ 35 };
}

void Humanoid::SetAnimation()
{
	//for (int i = 0;i < HP::HMAX;i++) {
	//	for (int j = 0;j < LOD::LMAX;j++) {
	//		for (int k = 0;k < ANIMATION::AMAX;i + k++) {

	//			switch (k)
	//			{
	//			case Enemy::MOVE:
	//				Model::SetAnimFrame(hModel_[i][j][k], 0, MOVEANIMFRAME , 1.0f);
	//				break;
	//			case Enemy::HIT:
	//				Model::SetAnimFrame(hModel_[i][j][k], 0, HITANIMFRAME, 1.0f);
	//				break;
	//			case Enemy::DEATH:
	//				Model::SetAnimFrame(hModel_[i][j][k], 0, DEATHANIMFRAME, 1.0f);
	//				break;
	//			default:
	//				break;
	//			}
	//		}
	//	}
	//}

	switch (ModelAnim_)
	{
	case Enemy::MOVE:
		Model::SetAnimFrame(hModel_[ModelHP_][ModelLOD_][ModelAnim_], 0, MOVEANIMFRAME, 1.0f);
		break;
	case Enemy::HIT:
		Model::SetAnimFrame(hModel_[ModelHP_][ModelLOD_][ModelAnim_], 0, HITANIMFRAME, 1.0f);
		break;
	case Enemy::DEATH:
		Model::SetAnimFrame(hModel_[ModelHP_][ModelLOD_][ModelAnim_], 0, DEATHANIMFRAME, 1.0f);
		break;
	default:
		break;
	}

	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 1.5f, 0), XMFLOAT3(3.0f, 3.0f, 3.0f));
	AddCollider(collision);
}

int Humanoid::GetDeathAnimFrame() const
{
	return DEATHANIMFRAME;
}

int Humanoid::GetHitFrame() const
{
	return HITANIMFRAME;
}

Humanoid::Humanoid(GameObject* parent)
	:Enemy(parent, "Humanoid")
{
}

Humanoid::~Humanoid()
{
}

void Humanoid::Release()
{
}
