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

namespace {
	const float PLAYTIME{ 600.0f };
	const int OBJECTNUM{ 5 }; //ロードするオブジェクトの数
	const float CLEARTIMER{ 5.0f };
	const float GAMEOVERTIMER{ 5.0f };
	const float CAMERAHEIGHT{ 20 };
}

void PlayScene::LoadObject()
{
	//順番にインスタンスを作る
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
	EmitterLoad();

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
		if (!isClear_ && !isGameOver_)
			PlayTimer_ += Time::DeltaTime();


		HUD* hud = FindGameObject<HUD>();
		hud->SetTimer(PlayTimer_);

		if (PlayTimer_ >= PLAYTIME) {
			//描画のみする
			SetChildFlags(0b10100);
			isClear_ = true;
		}

		if (isClear_) {

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

EmitterData PlayScene::GetEmitterData(std::string _objectName, std::string EmitterName)
{
	for (int i = 0; i < emitterList_.size(); i++)
	{
		if (emitterList_[i].objectName == _objectName && emitterList_[i].emitterName == EmitterName) {
			return emitterList_[i].emitterData;
		}
	}
}

void PlayScene::EmitterLoad()
{
	//なんかもっといい方法がある気がする
	enum EmitterFileData {
		OBJECTNAME,
		EMITTERNAME,
		FILENAME,
		POSITIONX,
		POSITIONY,
		POSITIONZ,
		POSITIONRNDX,
		POSITIONRNDY,
		POSITIONRNDZ,
		DIRECTIONX,
		DIRECTIONY,
		DIRECTIONZ,
		DIRECTIONRNDX,
		DIRECTIONRNDY,
		DIRECTIONRNDZ,
		SPEED,
		SPEEDRND,
		ACCEL,
		DELAY,
		NUMBER,
		GRAVITY,
		LIFETIME,
		COLORR,
		COLORG,
		COLORB,
		COLORA,
		DELTACOLORR,
		DELTACOLORG,
		DELTACOLORB,
		DELTACOLORA,
		ROTATEX,
		ROTATEY,
		ROTATEZ,
		ROTATERNDX,
		ROTATERNDY,
		ROTATERNDZ,
		SPINX,
		SPINY,
		SPINZ,
		SIZEX,
		SIZEY,
		SIZERNDX,
		SIZERNDY,
		SCALEX,
		SCALEY,
		ISBILLBOARD,
		LOOPCOUNT,
		MAX
	};

	CsvReader csv;
	csv.Load("Assets\\CSV\\EmitterFile.csv");

	//パーティクルのデータを読み込む
	for (int i = 1; i < csv.GetHeight(); i++) {
		EmitterFile ef;
		for (int j = 0; j < csv.GetWidth() || j < MAX; j++) {
			if(csv.GetString(j,i)=="def")
				continue; //defは無視
			switch (j)
			{
			case OBJECTNAME:
				ef.objectName = csv.GetString(j, i);
				break;
			case EMITTERNAME:
				ef.emitterName = csv.GetString(j, i);
				break;
			case FILENAME:
				ef.emitterData.textureFileName = "Assets\\Image\\Paticle\\" + csv.GetString(j, i) + ".png";
				break;
			case POSITIONX:
				ef.emitterData.position.x = csv.GetValue(j, i);
				break;
			case POSITIONY:
				ef.emitterData.position.y = csv.GetValue(j, i);
				break;
			case POSITIONZ:
				ef.emitterData.position.z = csv.GetValue(j, i);
				break;
			case POSITIONRNDX:
				ef.emitterData.positionRnd.x = csv.GetValue(j, i);
				break;
			case POSITIONRNDY:
				ef.emitterData.positionRnd.y = csv.GetValue(j, i);
				break;
			case POSITIONRNDZ:
				ef.emitterData.positionRnd.z = csv.GetValue(j, i);
				break;
			case DIRECTIONX:
				ef.emitterData.direction.x = csv.GetValue(j, i);
				break;
			case DIRECTIONY:
				ef.emitterData.direction.y = csv.GetValue(j, i);
				break;
			case DIRECTIONZ:
				ef.emitterData.direction.z = csv.GetValue(j, i);
				break;
			case DIRECTIONRNDX:
				ef.emitterData.directionRnd.x = csv.GetValue(j, i);
				break;
			case DIRECTIONRNDY:
				ef.emitterData.directionRnd.y = csv.GetValue(j, i);
				break;
			case DIRECTIONRNDZ:
				ef.emitterData.directionRnd.z = csv.GetValue(j, i);
				break;
			case SPEED:
				ef.emitterData.speed = csv.GetValue(j, i);
				break;
			case SPEEDRND:
				ef.emitterData.speedRnd = csv.GetValue(j, i);
				break;
			case ACCEL:
				ef.emitterData.accel = csv.GetValue(j, i);
				break;
			case DELAY:
				ef.emitterData.delay = csv.GetValue(j, i);
				break;
			case NUMBER:
				ef.emitterData.number = csv.GetValue(j, i);
				break;
			case GRAVITY:
				ef.emitterData.gravity = csv.GetValue(j, i);
				break;
			case LIFETIME:
				ef.emitterData.lifeTime = csv.GetValue(j, i);
				break;
			case COLORR:
				ef.emitterData.color.x = csv.GetValue(j, i);
				break;
			case COLORG:
				ef.emitterData.color.y = csv.GetValue(j, i);
				break;
			case COLORB:
				ef.emitterData.color.z = csv.GetValue(j, i);
				break;
			case COLORA:
				ef.emitterData.color.w = csv.GetValue(j, i);
				break;
			case DELTACOLORR:
				ef.emitterData.deltaColor.x = csv.GetValue(j, i);
				break;
			case DELTACOLORG:
				ef.emitterData.deltaColor.y = csv.GetValue(j, i);
				break;
			case DELTACOLORB:
				ef.emitterData.deltaColor.z = csv.GetValue(j, i);
				break;
			case DELTACOLORA:
				ef.emitterData.deltaColor.w = csv.GetValue(j, i);
				break;
			case ROTATEX:
				ef.emitterData.rotate.x = csv.GetValue(j, i);
				break;
			case ROTATEY:
				ef.emitterData.rotate.y = csv.GetValue(j, i);
				break;
			case ROTATEZ:
				ef.emitterData.rotate.z = csv.GetValue(j, i);	
				break;
			case ROTATERNDX:
				ef.emitterData.rotateRnd.x = csv.GetValue(j, i);
				break;
			case ROTATERNDY:
				ef.emitterData.rotateRnd.y = csv.GetValue(j, i);
				break;
			case ROTATERNDZ:
				ef.emitterData.rotateRnd.z = csv.GetValue(j, i);
				break;
			case SPINX:
				ef.emitterData.spin.x = csv.GetValue(j, i);
				break;
			case SPINY:
				ef.emitterData.spin.y = csv.GetValue(j, i);
				break;
			case SPINZ:
				ef.emitterData.spin.z = csv.GetValue(j, i);
				break;
			case SIZEX:
				ef.emitterData.size.x = csv.GetValue(j, i);
				break;
			case SIZEY:
				ef.emitterData.size.y = csv.GetValue(j, i);
				break;
			case SIZERNDX:
				ef.emitterData.sizeRnd.x = csv.GetValue(j, i);
				break;
			case SIZERNDY:
				ef.emitterData.sizeRnd.y = csv.GetValue(j, i);
				break;
			case SCALEX:
				ef.emitterData.scale.x = csv.GetValue(j, i);
				break;
			case SCALEY:
				ef.emitterData.scale.y = csv.GetValue(j, i);
				break;
			case ISBILLBOARD:
				if (csv.GetValue(j, i) >= 1)
					ef.emitterData.isBillBoard = true;
				else
					ef.emitterData.isBillBoard = false;
				break;
			case LOOPCOUNT:
				break;
			case MAX:
				break;
			default:
				break;
			}

		}

		emitterList_.push_back(ef);
	}

}