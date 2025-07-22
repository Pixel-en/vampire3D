#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"

const float TIMER{ 1.0f };

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent, "TitleScene"), hImage_(-1), hImageBlack_(-1)
{
}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("Assets\\Image\\Title.png");
	assert(hImage_ >= 0);
	hImageBlack_ = Image::Load("Assets\\Image\\UI\\BlackBack.png");
	assert(hImageBlack_ >= 0);
	Image::SetAlpha(hImageBlack_, 0); //黒画像のアルファ値を0にしておく

	//handle = TextFont::Load("Assets\\Kenney Pixel.tff");
	timer_ = TIMER;
	isbutton_ = false;

	hSE_ = Audio::Load("Assets\\Audio\\SE\\Choise.wav");
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)||Input::IsPadButtonDown(XINPUT_GAMEPAD_START)) {
		if (!isbutton_)
			Audio::Play(hSE_);

		isbutton_ = true;
	}
	if (isbutton_) {
		if (timer_ <= 0.0f) {
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SCENE_ID_PLAY);
		}
		else {
			timer_ -= Time::DeltaTime();
			//暗転のため画像のアルファ値を増やしていく
			Image::SetAlpha(hImageBlack_,Image::ALPHAMAX- Image::ALPHAMAX * timer_);
		}
	}
}

void TitleScene::Draw()
{
	//タイトル画像
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);

	//暗転のための黒画像
	Image::SetTransform(hImageBlack_, transform_);
	Image::Draw(hImageBlack_);
}

void TitleScene::Release()
{
}
