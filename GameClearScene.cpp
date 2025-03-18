#include "GameClearScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

GameClearScene::GameClearScene(GameObject* parent)
	:GameObject(parent, "GameClearScene"),image_(0)	
{
}

void GameClearScene::Initialize()
{
	image_=Image::Load("Assets\\/Image/GameClear.png");
	HandleCheck(image_);
}

void GameClearScene::Update()
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_START)) {
		SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
		sc->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameClearScene::Draw()
{
	Image::SetTransform(image_, transform_);
	Image::Draw(image_);
}

void GameClearScene::Release()
{
}
