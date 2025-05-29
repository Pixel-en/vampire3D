#include "PlayScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Audio.h"
#include "Engine/Input.h"

//オブジェクト
#include "Player.h"
#include "Field.h"
#include "EnemySpawn.h"
#include "EXPManager.h"
#include "HUD.h"
#include "PlayLoad.h"

namespace {
	const float PLAYTIME{ 900.0f };
}

void PlayScene::LoadResource()
{
	static int cnt= 0;

	switch (cnt)
	{
	case 0:
		Instantiate<Field>(this);
		break;
	case 1:
		Instantiate<EnemySpawn>(this);
		break;
	case 2:
		Instantiate<Player>(this);
		break;
	case 3:
		Instantiate<EXPManager>(this);
		break;
	case 4:
		Instantiate<HUD>(this);
		break;
	default:
		isLoaded_ = true;
		break;
	}
	//Instantiate<Field>(this);
	//Instantiate<EnemySpawn>(this);
	//Instantiate<Player>(this);
	//Instantiate<EXPManager>(this);
	//Instantiate<HUD>(this);
	cnt++;
	return;

}

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene")
{
	PlayTimer_ = 0.0f;
}

void PlayScene::Initialize()
{
	Instantiate<PlayLoad>(this);
	isLoaded_ = false;	

	hIntroSound_ = Audio::Load("Assets\\Audio\\BGM\\Play_intro.wav");
	HandleCheck(hIntroSound_, "プレイのイントロがない");
	hLoopSound_ = Audio::Load("Assets\\Audio\\BGM\\Play_loop.wav", true);
	HandleCheck(hLoopSound_, "プレイのループBGMがない");
}

void PlayScene::Update()
{
	if (!isLoaded_) {
		if (Input::IsKeyDown(DIK_SPACE)) {
			LoadResource();
			if (isLoaded_) {
				Audio::Play(hIntroSound_);
				PlayLoad* PL = FindGameObject<PlayLoad>();
				PL->KillMe();
			}
		}
	}
	else {
		if (!Audio::isPlaying(hIntroSound_) && !Audio::isPlaying(hLoopSound_)) {
			Audio::Play(hLoopSound_);
			Audio::Stop(hIntroSound_);
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
	Debug::Log(PlayTimer_, true);
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
