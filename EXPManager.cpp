#include "EXPManager.h"

namespace {
	const int SPAWNMAX{ 100 };
	const int EXPDISTRIBUTION{ 20 };
}

EXPManager::EXPManager(GameObject* parent)
	:GameObject(parent, "EXPManager")
{
	EXPStorage_ = 0;
}

EXPManager::~EXPManager()
{
}

void EXPManager::Initialize()
{
}

void EXPManager::SuperUpdate()
{
	for (auto it = EXPList_.begin(); it != EXPList_.end();) {
		if ((*it)->isGet()) {
			(*it)->KillMe();
			it = EXPList_.erase(it);
		}
		else
			it++;
	}
}

void EXPManager::Update()
{
	//経験値を出現させれなくなったら出現してるところに分配する
	if (EXPStorage_ >= 20) {
		int distriVal = EXPStorage_ / EXPDISTRIBUTION;	//分配する値を決める
		if (distriVal <= 0)	//もし分配値が0なら終了
			return;

		for (int i = 0; i < EXPDISTRIBUTION; i++) {
			int num = rand() % SPAWNMAX;	//乱数で分配先を決める
			//分配値が残りの値より大きいならすべて渡す
			if (EXPStorage_ < distriVal) {
				EXPList_[num]->AddEXP(EXPStorage_);
				EXPStorage_ = 0;
			}
			else {
				EXPList_[num]->AddEXP(distriVal);
				EXPStorage_ -= distriVal;
			}
		}
	}
}

void EXPManager::Draw()
{
}

void EXPManager::Release()
{
}

void EXPManager::SpawnEXP(XMFLOAT3 pos, int _exp)
{
	if (EXPList_.size() >= SPAWNMAX) {
		EXPStorage_ += _exp;
		return;
	}

	EXP* exp = Instantiate<EXP>(GetParent());
	exp->SetStatus(pos, _exp);
	EXPList_.push_back(exp);

}
