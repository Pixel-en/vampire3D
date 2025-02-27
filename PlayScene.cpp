#include "PlayScene.h"

//オブジェクト
#include "Player.h"
#include "Field.h"
#include "EnemySpawn.h"
#include "EXPManager.h"
#include "HUD.h"
#include "Engine/TextFont.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene")
{
	time_ = 0;
}

void PlayScene::Initialize()
{
	Instantiate<Field>(this);
	Instantiate<EnemySpawn>(this);
	Instantiate<Player>(this);
	Instantiate<EXPManager>(this);
	Instantiate<HUD>(this);
}

void PlayScene::Update()
{
	int num = GetChildList()->size();
	SwapChildList("HUD", num);
	time_ += Time::DeltaTime();
}

void PlayScene::Draw()
{
	TextFont::Draw(std::to_string(time_), { 500,200 });
}

void PlayScene::Release()
{
}
