#pragma once
#include "WeaponObject.h"

//cLaserを管理する(レベルなど)
class Laser :public WeaponObject
{

	//実際に動くオブジェクト
	class cLaser : public WeaponObject
	{
		float angle_;
		float BiggerWaittimer_;	//レーザーが大きくなるまでのタイマー

		void Move() override;

		void ResetSub() override;

		void AddBullet()  override {};
	public:
		cLaser(GameObject* parent);

		~cLaser();

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

	std::vector<Laser::cLaser*> List_;

	void AddBullet() override;

	//レーザーの方向を決める
	void LaserDir();
public:
	Laser(GameObject* parent);

	~Laser();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

