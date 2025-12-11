#pragma once
#include "Engine/GameObject.h"

//プレイシーンのロード画面
class PlayLoad :public GameObject
{
	//背景用ハンドル
	int hBGImage_;
	//ロード中の画像ハンドル
	int hFrameImage_;
	int hBackImage_;

	int hBarImage_;

	int hStartImage_;

	Transform FrameTrans_;
	Transform BarTrans_;
	Transform StartTrans_;

	//ロード開始までのタイマー
	float WaitTimer_;
	bool isStart_;

public:
	PlayLoad(GameObject* parent);

	~PlayLoad();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	/// <summary>
	/// バーの長さを設定
	/// </summary>
	/// <param name="scale">長さ</param>
	void SetBarScale(float scale);

	/// <summary>
	/// スタートフラグが立っているかどうか
	/// </summary>
	/// <returns>スタートフラグ</returns>
	bool GetIsStart() { return isStart_; }
};

