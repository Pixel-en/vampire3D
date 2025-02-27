#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Image.h"

static int handle = 0;

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),hImage_(-1)
{
}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("Assets\\Image\\Title.jpg");
	assert(hImage_ >= 0);

	//handle = TextFont::Load("Assets\\Kenney Pixel.tff");
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
		sc->ChangeScene(SCENE_ID_PLAY);
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
