#include "PlayScene.h"
#include "Engine/SceneManager.h"

//オブジェクト
#include "Player.h"
#include "Field.h"
#include "EnemySpawn.h"
#include "EXPManager.h"
#include "HUD.h"

namespace {
	const float PLAYTIME{ 900.0f };
}

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene")
{
	PlayTimer_ = 0.0f;
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
	PlayTimer_ += Time::DeltaTime();

	HUD* hud = FindGameObject<HUD>();
	hud->SetTimer(PlayTimer_);

	if (PlayTimer_ >= PLAYTIME) {
		SceneManager* scene = GetRootJob()->FindGameObject<SceneManager>();
		scene->ChangeScene(SCENE_ID_GAMECLEAR);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
