#include "PlayScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Audio.h"

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

	hIntroSound_ = Audio::Load("Assets\\Audio\\BGM\\Play_intro.wav");
	HandleCheck(hIntroSound_, "プレイのイントロがない");
	hLoopSound_ = Audio::Load("Assets\\Audio\\BGM\\Play_loop.wav", true);
	HandleCheck(hLoopSound_, "プレイのループBGMがない");
	Audio::Play(hIntroSound_);
}

void PlayScene::Update()
{
	if (!Audio::isPlaying(hIntroSound_)&&!Audio::isPlaying(hLoopSound_)) {
		Audio::Play(hLoopSound_);
		//Audio::Stop(hIntroSound_);
	}

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
