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

		void SetAngle(int _degrees) { angle_ = _degrees; };
	};


	void AddBullet() override;

	//角度を調整する
	void AdjustedAngle();
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

