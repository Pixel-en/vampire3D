#pragma once
#include "WeaponObject.h"

//cKnifeを管理する(レベルなど)
class Knife :public WeaponObject
{
	//実際に動くオブジェクト
	class cKnife : public WeaponObject
	{
		float angleX_;

		void Move() override;

		void ResetSub() override;

		void AddBullet() override {};
	public:
		cKnife(GameObject* parent);

		~cKnife();

		//初期化
		void Initialize() override;

		//更新
		//void Update() override;

		//描画
		void Draw() override;

		//開放
		void Release() override;
	};

	void AddBullet() override;
public:
	Knife(GameObject* parent);

	~Knife();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

