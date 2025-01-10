#pragma once
#include "Engine/GameObject.h"
namespace {
	const int FBXNUM{ 4 };
}
//経験値
class EXP :public GameObject
{
	int hModel_;
	int expValue_;	//経験値量
	float speed_;

	bool dead_;	//取得された

	enum EXPVAL
	{
		SMALL = 2,
		MEDIUM = 9,
		LARGE = 23,
		MAX,
	};

	/// <summary>
	/// モデルをロードする
	/// </summary>
	void LoadModel();

public:
	EXP(GameObject* parent);

	~EXP();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	/// <summary>
	/// 初期値を設定
	/// </summary>
	/// <param name="_pos">出現場所</param>
	/// <param name="_exp">経験値量</param>
	void SetStatus(XMFLOAT3 _pos, int _exp);

	/// <summary>
	/// 加算経験値
	/// </summary>
	/// <param name="_exp">経験値量</param>
	void AddEXP(int _exp);

	/// <summary>
	/// 取得されたかどうか
	/// </summary>
	/// <returns>されたらtrue</returns>
	bool isGet() { return dead_; };

	void OnCollision(GameObject* pTarget) override;
};

