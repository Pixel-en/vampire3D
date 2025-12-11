#include "GameOverScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"),image_(-1)
{
}

void GameOverScene::Initialize()
{
	image_ = Image::Load("Assets\\Image\\GameOver.png");
	HandleCheck(image_);
}

void GameOverScene::Update()
{
	//ƒV[ƒ“‘JˆÚ
	if (Input::IsKeyDown(DIK_RETURN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_START)) {
		SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
		sc->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(image_, transform_);
	Image::Draw(image_);
}

void GameOverScene::Release()
{
}
