#include "PlayLoad.h"
#include "Engine/Image.h"
#include "Engine/TextFont.h"

namespace {
	const XMFLOAT3 FRAMEPOS{ 0.65, -0.8, 0 };
	const XMFLOAT3 BARPOS{ 0.35, -0.8, 0 };
	const XMFLOAT2 TEXTPOS{ 0.5f, -0.85f }; //テキストの位置
	const float WAITTIME{ 1.0f }; //ロード開始までの待機時間
}

PlayLoad::PlayLoad(GameObject* parent)
	:GameObject(parent, "PlayLoad"), hBGImage_(-1), hFrameImage_(-1), hBarImage_(-1)
{
}

PlayLoad::~PlayLoad()
{
}

void PlayLoad::Initialize()
{
	hBGImage_ = Image::Load("Assets\\Image\\UI\\BlackBack.png");
	HandleCheck(hBGImage_, "ロード画面の背景画像がない");
	hFrameImage_ = Image::Load("Assets\\Image\\UI\\ProgressFrame.png");
	HandleCheck(hFrameImage_, "ロード中の画像がない");
	hBackImage_ = Image::Load("Assets\\Image\\UI\\ProgressBack.png");
	HandleCheck(hBackImage_, "ロード中のバーの背景画像がない");
	hBarImage_ = Image::Load("Assets\\Image\\UI\\ProgressBar.png");
	HandleCheck(hBarImage_, "ロード中のバー画像がない");
	hStartImage_ = Image::Load("Assets\\Image\\UI\\startMenu.png");
	HandleCheck(hStartImage_);

	FrameTrans_.position_ = FRAMEPOS;
	BarTrans_.position_ = BARPOS;
	//長さを0にする
	BarTrans_.scale_ = { 0.0f, 1.0f, 1.0f };
	WaitTimer_ = WAITTIME;
	isStart_ = false;
	StartTrans_.position_ = { 0,0,0 };
	StartTrans_.scale_ = { 2.0,2.0,0 };
}

void PlayLoad::Update()
{
	if (WaitTimer_ > 0.0f) {
		//ロード開始までの待機時間
		WaitTimer_ -= Time::DeltaTime();
		if (WaitTimer_ <= 0.0f) {
			isStart_ = true; //ロード開始
		}
		return;
	}

}

void PlayLoad::Draw()
{

	Image::SetTransform(hBGImage_, transform_);
	Image::Draw(hBGImage_);
	Image::SetTransform(hBackImage_, FrameTrans_);
	Image::Draw(hBackImage_);
	Image::SetTransform(hBarImage_, BarTrans_);
	Image::Draw(hBarImage_);
	Image::SetTransform(hFrameImage_, FrameTrans_);
	Image::Draw(hFrameImage_);
	Image::SetTransform(hStartImage_, StartTrans_);

	FontData data;
	data.font = TextFont::GetFontName(FontList::Makinas);
	data.Color = D2D1::ColorF(255, 255, 255);
	data.fontSize = 25;
	std::string text = "LOADING...";
	if (BarTrans_.scale_.x >= 1.0f) {
		text = "LOADING COMPLETE!";
		Image::Draw(hStartImage_);
	}

	TextFont::Draw(text, { 900, 590 }, data);
}

void PlayLoad::Release()
{
}

void PlayLoad::SetBarScale(float scale)
{
	BarTrans_.scale_.x = scale;
	if (BarTrans_.scale_.x > 1.0f) {
		BarTrans_.scale_.x = 1.0f; //最大値を1.0に制限
	}
}
