#pragma once
#include "WeaponObject.h"
#include <list>

//cBombを管理する(レベルなど)
class Bomb :public WeaponObject
{
	//実際に動くオブジェクト
	class cBomb : public WeaponObject
	{
		void Move() override;

		void ResetSub() override;

		void AddBullet() override {};
	public:
		cBomb(GameObject* parent);

		~cBomb();

		//初期化
		void Initialize() override;

		//更新
		void Update() override;

		//描画
		void Draw() override;

		//開放
		void Release() override;
	};

	std::list<Bomb::cBomb*> List_;

	void AddBullet() override;
public:
	Bomb(GameObject* parent);

	~Bomb();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

