#pragma once
#include "Enemy.h"
class Humanoid:public Enemy
{
	/// <summary>
	/// アニメーションをセットする
	/// </summary>
	void SetAnimation() override;
	/// <summary>
	/// デスアニメーションのフレームを取得
	/// </summary>
	/// <returns>フレーム数</returns>
	int GetDeathAnimFrame() const override;

	/// <summary>
	/// 火っとアニメーションのフレームを取得
	/// </summary>
	/// <returns>フレーム数</returns>
	int GetHitFrame() const override;
public:

	Humanoid(GameObject* parent);

	~Humanoid();

	//開放
	void Release() override;
};

