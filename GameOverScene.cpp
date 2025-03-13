#include "GameOverScene.h"
#include "Engine/Image.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"),image_(-1)
{
}

void GameOverScene::Initialize()
{
	image_ = Image::Load("Assets\\Image\\GameOver.jpg");
	HandleCheck(image_);
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
	Image::SetTransform(image_, transform_);
	Image::Draw(image_);
}

void GameOverScene::Release()
{
}
