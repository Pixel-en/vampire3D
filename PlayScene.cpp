#include "PlayScene.h"

//オブジェクト
#include "Player.h"
#include "Field.h"
#include "EnemySpawn.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene")
{
}

void PlayScene::Initialize()
{
	Instantiate<Field>(this);
	Instantiate<EnemySpawn>(this);
	Instantiate<Player>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
