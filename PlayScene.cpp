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
	const float CLEARTIMER{ 3.0f };
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
	isClear_ = false;
	clearTimer_ = CLEARTIMER;
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
	//ロードが終わってないなら
	if (!isLoaded_) {
		PlayLoad* PL = FindGameObject<PlayLoad>();
		if (PL->GetIsStart()) {
			//インスタンスを作る
			LoadObject();
			//更新と描画をしないようにする
			SetChildFlags(0b10001);
			//PlayLoadのみ描画と交信を行う
			PL->SetFlags(0b11101);
			//準備の割合を送る
			PL->SetBarScale((float)LoadCount_ / OBJECTNUM);
			//準備が完了したら
			if (LoadCount_ >= OBJECTNUM) {
				if (Input::IsKeyDown(DIK_RETURN) || Input::IsPadAllButtonDown()) {
					isLoaded_ = true;
					//描画と更新をするようにする
					SetChildFlags(0b11101);
					//ロード画面は殺す
					PL->KillMe();
					//BGMをつける
					Audio::Play(hIntroSound_);
				}
			}
		}
	}
	else {
		if (!isClear_)
			PlayTimer_ += Time::DeltaTime();


		HUD* hud = FindGameObject<HUD>();
		hud->SetTimer(PlayTimer_);

		if (PlayTimer_ >= PLAYTIME) {
			//描画のみする
			SetChildFlags(0b10100);
			hud->SetClearFlag(true);
			isClear_ = true;
		}

		if (isClear_) {
			if (clearTimer_ <= 0.0f) {
				SceneManager* scene = GetRootJob()->FindGameObject<SceneManager>();
				scene->ChangeScene(SCENE_ID_GAMECLEAR);
			}
			else {
				clearTimer_ -= Time::DeltaTime();
			}
		}
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
