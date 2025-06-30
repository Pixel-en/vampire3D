#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Image.h"

const float TIMER{ 1.0f };

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),hImage_(-1)
{
}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("Assets\\Image\\Title.png");
	assert(hImage_ >= 0);

	//handle = TextFont::Load("Assets\\Kenney Pixel.tff");
	timer_ = TIMER;
	isbutton_ = false;
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)||Input::IsPadButtonDown(XINPUT_GAMEPAD_START)) {
		isbutton_ = true;
	}
	if (isbutton_) {
		if (timer_ <= 0.0f) {
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SCENE_ID_PLAY);
		}
		else {
			timer_ -= Time::DeltaTime();
		}
	}
}

void TitleScene::Draw()
{
	Image::SetTransform(hImage_, transform_);
	Image::Draw(hImage_);
}

void TitleScene::Release()
{
}
