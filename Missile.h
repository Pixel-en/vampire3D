#pragma once
#include "WeaponObject.h"

//cMissileを管理する(レベルなど)
class Missile :public WeaponObject
{

	//実際に動くオブジェクト
	class cMissile : public WeaponObject
	{
		float UpTimer_;	//上昇用のタイマー
		bool Search_;
		XMFLOAT3 targetpos_;


		void Move() override;

		void ResetSub() override;

		void AddBullet()  override {};

		void Penetration() override;

		XMFLOAT3 searchEnemy();
	public:
		cMissile(GameObject* parent);

		~cMissile();

		//初期化
		void Initialize() override;

		//更新
		//void Update() override;

		//描画
		void Draw() override;

		//開放
		void Release() override;
	};

	std::vector<Missile::cMissile*> List_;

	void AddBullet() override;

public:
	Missile(GameObject* parent);

	~Missile();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

};

