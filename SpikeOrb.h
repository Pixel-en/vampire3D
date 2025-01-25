#pragma once
#include "WeaponObject.h"

//cSpikeOrbを管理する(レベルなど)
class SpikeOrb :public WeaponObject
{

	//実際に動くオブジェクト
	class cSpikeOrb : public WeaponObject
	{
		float angle_;

		Transform Rottransform_;

		void Move() override;

		void ResetSub() override;

		void AddBullet() override {};
	public:
		cSpikeOrb(GameObject* parent);

		~cSpikeOrb();

		//初期化
		void Initialize() override;

		//更新
		//void Update() override;

		//描画
		void Draw() override;

		//開放
		void Release() override;

		//当たり判定
		void OnCollision(GameObject* pTarget) override;
	};

	std::vector<SpikeOrb::cSpikeOrb*> List_;

	void AddBullet() override;
public:
	SpikeOrb(GameObject* parent);

	~SpikeOrb();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

