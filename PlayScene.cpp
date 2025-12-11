#include "PlayScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Audio.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/CsvReader.h"

//オブジェクト
#include "Player.h"
#include "Field.h"
#include "EnemySpawn.h"
#include "EXPManager.h"
#include "HUD.h"
#include "PlayLoad.h"
#include "Effect.h"

namespace {
	const float PLAYTIME{ 600.0f };
	const float CLEARTIMER{ 5.0f };
	const float GAMEOVERTIMER{ 5.0f };
	const float CAMERAHEIGHT{ 20 };
	int OBJECTNUM;	//ロードするオブジェクトの数
}

void PlayScene::LoadObject()
{
	enum {
		EXPMANAGER,
		FIELD,
		ENEMYSPAWN,
		PLAYER,
		EFFECT,
		HUD_,
		MAX
	};
	OBJECTNUM = MAX;

	//順番にインスタンスを作る
	switch (LoadCount_)
	{
	case EXPMANAGER:
		Instantiate<EXPManager>(this);
		break;
	case FIELD:
		Instantiate<Field>(this);
		break;
	case ENEMYSPAWN:
		Instantiate<EnemySpawn>(this);
		break;
	case PLAYER:
		Instantiate<Player>(this);
		break;
	case EFFECT:
		Instantiate<Effect>(this);
		break;
	case HUD_:
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
	gameOverTimer_ = GAMEOVERTIMER;
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

	VFX::VFXSTOP();

}

void PlayScene::Update()
{
	//ロードが終わってないなら
	if (!isLoaded_) {
		PlayLoad* PL = FindGameObject<PlayLoad>();
		//nullチェック
		if (PL == nullptr) {
			//PLがなくてカウントが超えているならロード完了してるはず
			if (LoadCount_ >= OBJECTNUM) {
				isLoaded_ = true;
				//描画と更新をするようにする
				SetChildFlags(0b11101);
				//BGMをつける
				Audio::Play(hIntroSound_);
			}
			else
				NullCheck(PL);
		}
		//インスタンスがあるなら
		else {
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
						//ロード画面は殺す
						PL->KillMe();
					}
				}
			}
		}
	}

	else {
		//VFX開始
		VFX::VFXSTART();

		//タイマー更新
		if (!isClear_ && !isGameOver_)
			PlayTimer_ += Time::DeltaTime();

		//HUD更新
		HUD* hud = FindGameObject<HUD>();
		hud->SetTimer(PlayTimer_);

		if (PlayTimer_ >= PLAYTIME) {
			//描画のみする
			SetChildFlags(0b10100);
			isClear_ = true;
		}

		if (isClear_) {
			//Clearと出してシーン遷移
			hud->SetSceneMessage("Clear!!");

			if (clearTimer_ <= 0.0f) {
				SceneManager* scene = GetRootJob()->FindGameObject<SceneManager>();
				scene->ChangeScene(SCENE_ID_GAMECLEAR);
			}
			else {
				clearTimer_ -= Time::DeltaTime();
			}
		}

		if (isGameOver_) {
			//GameOverと出してシーン遷移
			hud->SetSceneMessage("GameOver!");

			if (gameOverTimer_ <= 0.0f) {
				SceneManager* scene = GetRootJob()->FindGameObject<SceneManager>();
				scene->ChangeScene(SCENE_ID_GAMEOVER);
			}
			else {
				gameOverTimer_ -= Time::DeltaTime();
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

void PlayScene::PlayerDead()
{
	//フラグを立てる
	isGameOver_ = true;
	Player* player = FindGameObject<Player>();

	//プレイヤーの真上当たり
	Camera::SetPosition({ player->GetWorldPosition().x,player->GetWorldPosition().y+CAMERAHEIGHT,player->GetWorldPosition().z-1});
	//ターゲットはプレイヤーのいた場所
	Camera::SetTarget(player->GetWorldPosition()); 

}