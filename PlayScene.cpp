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
	const float PLAYTIME{ 600.0f };
	const int OBJECTNUM{ 5 }; //ロードするオブジェクトの数
}

void PlayScene::LoadObject()
{

	switch (LoadCount_)
	{
	case 0:
		Instantiate<EXPManager>(this);
		break;
	case 1:
		Instantiate<Field>(this);
		break;
	case 2:
		Instantiate<EnemySpawn>(this);
		break;
	case 3:
		Instantiate<Player>(this);
		break;
	case 4:
		Instantiate<HUD>(this);
		break;
	default:
		break;
	}

	LoadCount_++;

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
	LoadCount_ = 0;
}

void PlayScene::SuperUpdate()
{
	if (isLoaded_) {
		//イントロが終わったらループに入る
		if (!Audio::isPlaying(hIntroSound_) && !Audio::isPlaying(hLoopSound_)) {
			Audio::Play(hLoopSound_);
			Audio::Stop(hIntroSound_);
		}


		//HUDクラスを常に前に出す
		int num = GetChildList()->size();
		SwapChildList("HUD", num);
	}

}

void PlayScene::Update()
{
	if (!isLoaded_) {
		PlayLoad* PL = FindGameObject<PlayLoad>();
		if (PL->GetIsStart()) {
			LoadObject();
			SetChildFlags(0b10001);
			PL->SetFlags(0b11101);
			PL->SetBarScale((float)LoadCount_ / OBJECTNUM);
			if (LoadCount_ >= OBJECTNUM) {
				if (Input::IsKeyDown(DIK_SPACE)) {
					isLoaded_ = true;
					SetChildFlags(0b11101);
					PL->KillMe();
					Audio::Play(hIntroSound_);
				}
			}
		}
	}
	else {

		PlayTimer_ += Time::DeltaTime();


		HUD* hud = FindGameObject<HUD>();
		hud->SetTimer(PlayTimer_);

		if (PlayTimer_ >= PLAYTIME) {
			SceneManager* scene = GetRootJob()->FindGameObject<SceneManager>();
			scene->ChangeScene(SCENE_ID_GAMECLEAR);
		}
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
