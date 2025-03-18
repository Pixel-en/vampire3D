#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/TextFont.h"

#include "Player.h"
#include "EnemySpawn.h"

#include "Knife.h"
#include "PoisonThrow.h"
#include "SpikeOrb.h"
#include "Missile.h"
#include "Laser.h"
#include "Bomb.h"

namespace {
	namespace RADAR {
		const float RADARSCALE{ 1000.0f };			//レーダーのサイズに縮小
		const float RADARRANGE{ 14.0f / 100.0f };	//レーダーの感知範囲
		const XMFLOAT3 RADARPOS{ 0.84f,-0.72f,0.0f };
		const int RADARALPHA{ 200 };
	}
	namespace LEVEL {
		const float LEVELGAUGEYPOS{ 0.955f };
		const float LEVELGAUGEBARXPOS{ -1.0f };
		const int WEAPONCHOICEVAL{ 4 };
	}
}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD")
{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	RadarInitialize();
	LevelInitialize();
}

void HUD::SuperUpdate()
{
	LevelSuperUpdate();
}

void HUD::Update()
{
	LevelUpdate();
	RadarUpdate();
}

void HUD::Draw()
{
	TimerDraw();
	RadarDraw();
	LevelDraw();
}

void HUD::Release()
{
}

void HUD::RadarInitialize()
{
	hRadar_ = -1;
	hREnemy_ = -1;
	RadarTransform_ = transform_;
	REnemyTransform_ = transform_;
	RPlayerTransform_ = transform_;

	hRadar_ = Image::Load("Assets\\Image\\Radar192.png");
	assert(hRadar_ >= 0);
	hREnemy_ = Image::Load("Assets\\Image\\RadarEnemy.png");
	assert(hREnemy_ >= 0);
	hRPlayer_ = Image::Load("Assets\\Image\\RadarPlayer.png");
	assert(hRPlayer_ >= 0);

}

void HUD::RadarUpdate()
{
	Player* player = GetParent()->FindGameObject<Player>();
	assert(player != nullptr);

	//敵のリストを取得
	EnemySpawn* ep = GetParent()->FindGameObject<EnemySpawn>();
	assert(ep != nullptr);
	auto EnemyList = ep->GetEnemyList();

	//敵がいないなら戻る
	if (EnemyList.empty())
		return;

	//一度リストを空にする
	REnemyPosList_.clear();

	for (auto I : EnemyList) {

		//プレイヤーと敵の距離が一定以上外なら表示しない
		float EPDistance = I->GetPosition() - player->GetPosition();

		if (EPDistance <= -RADAR::RADARRANGE * RADAR::RADARSCALE || EPDistance >= RADAR::RADARRANGE * RADAR::RADARSCALE)
			continue;

		//画像は2次元でxy座標なのでyとz入れ替え
		XMFLOAT3 EPos = { I->GetPosition().x - player->GetPosition().x,I->GetPosition().z - player->GetPosition().z, I->GetPosition().y - player->GetPosition().y };
		//レーダー用に比率を変えずに小さくする
		EPos.x = EPos.x / RADAR::RADARSCALE;
		EPos.y = EPos.y / RADAR::RADARSCALE * (screenWidth / screenHeight);		//画面の比率によって変わるため比率を合わせる

		REnemyPosList_.push_back(EPos);
	}

	//ここもzとy入れ替え+逆回転になってるので－
	RPlayerTransform_.rotate_.z = -player->GetRotate().y;
}

void HUD::RadarDraw()
{
	//透明度変更
	Image::SetAlpha(hRadar_, RADAR::RADARALPHA);
	Image::SetAlpha(hREnemy_, RADAR::RADARALPHA);
	Image::SetAlpha(hRPlayer_, RADAR::RADARALPHA);

	//レーダーの一を右下に
	RadarTransform_.position_ = RADAR::RADARPOS;
	RPlayerTransform_.position_ = RADAR::RADARPOS;

	//レーダーの背景
	Image::SetTransform(hRadar_, RadarTransform_);
	Image::Draw(hRadar_);

	//レーダーのプレイヤー
	Image::SetTransform(hRPlayer_, RPlayerTransform_);
	Image::Draw(hRPlayer_);

	//レーダーの敵
	for (auto I : REnemyPosList_) {
		REnemyTransform_.position_ = I + RADAR::RADARPOS;
		Image::SetTransform(hREnemy_, REnemyTransform_);
		Image::Draw(hREnemy_);
	}

}

//レベルアップした時の処理
void HUD::LevelUP()
{
	LGBarTransform_.scale_ = { 1,1,1 };

	Pause_ = true;

	//リストの整理
	for (auto itr = WeaponList_.begin(); itr != WeaponList_.end();) {
		WeaponObject* obj = (WeaponObject*)GetParent()->FindChildObject((*itr).name_);
		if (obj != nullptr) {
			if (obj->GetLv() >= (*itr).MaxLevel_) {
				itr = WeaponList_.erase(itr);
				continue;
			}
		}
		itr++;
	}

	WeaponRoll();
}

void HUD::WeaponRoll()
{

	if (WeaponList_.size() >= LEVEL::WEAPONCHOICEVAL) {
		while (true)
		{
			RollListNum_.clear();

			for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
				RollListNum_.insert(rand() % WeaponList_.size());
			}
			if (RollListNum_.size() == LEVEL::WEAPONCHOICEVAL)
				break;
		}
	}

}

void HUD::LevelInitialize()
{
	hLevelBack_ = -1;
	hLevelGaugeFrame_ = -1;
	hLevelGaugeBar_ = -1;
	hLevelFrameImage_ = -1;
	hLevelCursorImage_ = -1;

	LBackTransform_ = transform_;
	LGFrameTransform_ = transform_;
	LGBarTransform_ = transform_;
	LCursorTransform_ = transform_;

	LGFrameTransform_.position_ = { 0,LEVEL::LEVELGAUGEYPOS,0 };
	LGBarTransform_.position_ = { LEVEL::LEVELGAUGEBARXPOS,LEVEL::LEVELGAUGEYPOS,0 };

	hLevelBack_ = Image::Load("Assets\\Image\\LevelUpBackGround0.6.png");
	HandleCheck(hLevelBack_);
	hLevelGaugeFrame_ = Image::Load("Assets\\Image\\LevelFrame.png");
	HandleCheck(hLevelGaugeFrame_);
	hLevelGaugeBar_ = Image::Load("Assets\\Image\\LevelBar.png");
	HandleCheck(hLevelGaugeBar_);
	hLevelCursor_ = Image::Load("Assets\\Image\\LevelCursor.png");
	HandleCheck(hLevelCursor_);
	hLevelFrameImage_ = Image::Load("Assets\\Image\\LevelUpFrame.png");
	HandleCheck(hLevelFrameImage_);
	hLevelCursorImage_ = Image::Load("Assets\\Image\\LevelUpCursor.png");
	HandleCheck(hLevelCursorImage_);

	CsvReader csv,effect;
	if (!csv.Load("Assets\\CSV\\WeaponList.csv"))
		return;
	if (!effect.Load("Assets\\CSV\\WeaponLevelText.csv"))
		return;

	for (int i = 1; i < csv.GetHeight(); i++) {
		WeaponList_.push_back({ csv.GetString(0, i),(int)csv.GetValue(1, i), (int)csv.GetValue(2, i)});
		//pop_backができるので逆から入れてみる
		for (int j = WeaponList_[i - 1].MaxLevel_ - 1 - 1; j >= 0; j--) {
			WeaponList_[i - 1].instruction_.push_back(csv.GetString(3 + j, i));
		}
		for (int j = 2; j < effect.GetWidth(); j++) {
			WeaponList_[i - 1].EffectText_[j - 2] = effect.GetString(j, i);
		}
	}


	StickTriggerY_ = false;
}

void HUD::LevelSuperUpdate()
{
	if (Pause_) {

		if ((Input::IsKeyDown(DIK_UP) || Input::GetPadStickL().y>=0.5f)&&!StickTriggerY_)
		{
			levelCursor_--;
			StickTriggerY_ = true;
		}
		else if ((Input::IsKeyDown(DIK_DOWN) || Input::GetPadStickL().y <= -0.5f) && !StickTriggerY_){
			levelCursor_++;
			StickTriggerY_ = true;
		}
		if(Input::GetPadStickL().y >= -0.5f && Input::GetPadStickL().y <= 0.5f)
			StickTriggerY_ = false;

		levelCursor_ = levelCursor_ % LEVEL::WEAPONCHOICEVAL;
		if (levelCursor_ < 0)
			levelCursor_ = 3;

		//LCursorTransform_.position_.y = 0.6 + -0.1 * levelCursor_;
		LCursorTransform_.position_= { 330 / (screenWidth / 2.0f),(310 - (80 + levelCursor_ * 170)) / (screenHeight / 2.0f) ,0 };
		if (Input::IsKeyDown(DIK_RETURN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B)) {
			auto itr = RollListNum_.begin();
			std::advance(itr, levelCursor_);
			ObtainWeapon((*itr));
		}
	}
}

void HUD::LevelUpdate()
{
	Pause_ = false;
	levelCursor_ = 0;

	Player* player = GetParent()->FindGameObject<Player>();
	float current = player->GetStatus().currentExp_;
	float next = player->GetStatus().nextLvExp_;

	float ratio = current / next;

	LGBarTransform_.scale_ = { ratio,1,1 };
}

void HUD::LevelDraw()
{
	Image::SetTransform(hLevelGaugeFrame_, LGFrameTransform_);
	Image::Draw(hLevelGaugeFrame_);

	Image::SetTransform(hLevelGaugeBar_, LGBarTransform_);
	Image::Draw(hLevelGaugeBar_);

	if (Pause_) {
		Image::SetTransform(hLevelBack_, LBackTransform_);
		Image::Draw(hLevelBack_);
		
		//Image::SetTransform(hLevelCursor_, LCursorTransform_);
		//Image::Draw(hLevelCursor_);
		Image::SetTransform(hLevelCursorImage_, LCursorTransform_);
		Image::Draw(hLevelCursorImage_);

		Player* player = GetParent()->FindGameObject<Player>();

		//レベルアップで選択できる武器
		auto itr = RollListNum_.begin();
		for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
			//														初期値-画像の縦の長さ/2+i*画像の長さ+バッファ
			LFrameTransform_.position_ = { 330 / (screenWidth / 2.0f),(310-(80+i*170)) / (screenHeight / 2.0f) ,0 };
			Image::SetTransform(hLevelFrameImage_, LFrameTransform_);
			Image::Draw(hLevelFrameImage_);

			std::string level = "new";
			int Lv = 0;
			FontData data;
			data.font = TextFont::GetFontName(FontList::Gkktt);
			data.Color = D2D1::ColorF(255, 255, 255);
			data.fontSize = 30;

			//武器の名前
			TextFont::Draw(WeaponList_[(*std::next(itr, i))].name_.c_str(), { 720,70.0f + (i * 170) }, data);

			//武器のレベル
			for (int j = 0; j < player->MyWeaponList_.size(); j++) {
				if (WeaponList_[(*std::next(itr, i))].name_ == player->MyWeaponList_[j]->GetObjectName()) {
					level = "Level:" + std::to_string(player->MyWeaponList_[j]->GetLv() + 1);
					Lv = player->MyWeaponList_[j]->GetLv();
					break;
				}
			}

			TextFont::Draw(WeaponList_[(*std::next(itr, i))].EffectText_[Lv].c_str(), { 720,120.0f + (i * 170) }, {1250,140.0f+(i*170)}, data);

			TextFont::Draw(level.c_str(), { 1100,70.0f + (i * 170) }, data);
			
		}
	}
}

void HUD::ObtainWeapon(int _num)
{
	Player* player = GetParent()->FindGameObject<Player>();
	if (player == nullptr)
		return;


	switch (WeaponList_[_num].num_)
	{
	case 0: {
		Knife* knife = GetParent()->FindGameObject<Knife>();
		if (knife == nullptr) {
			knife = Instantiate<Knife>(GetParent());
			player->MyWeaponList_.push_back(knife);
		}
		else {
			knife->LevelUp(WeaponList_[_num].instruction_.back());
		}
	}
		break;
	case 1: {
		PoisonThrow* poison = GetParent()->FindGameObject<PoisonThrow>();
		if (poison == nullptr) {
			poison = Instantiate<PoisonThrow>(GetParent());
			player->MyWeaponList_.push_back(poison);
		}
		else {
			poison->LevelUp(WeaponList_[_num].instruction_.front());
		}
	}
		break;
	case 2:
	{
		SpikeOrb* spike = GetParent()->FindGameObject<SpikeOrb>();
		if (spike == nullptr) {
			spike = Instantiate<SpikeOrb>(GetParent());
			player->MyWeaponList_.push_back(spike);
		}
		else {
			spike->LevelUp(WeaponList_[_num].instruction_.front());
		}
	}
		break;
	case 3:	
	{
		Missile* missile = GetParent()->FindGameObject<Missile>();
		if (missile == nullptr) {
			missile = Instantiate<Missile>(GetParent());
			player->MyWeaponList_.push_back(missile);
		}
		else {
			missile->LevelUp(WeaponList_[_num].instruction_.front());
		}
	}
		break;
	case 4:
	{
		Laser* laser = GetParent()->FindGameObject<Laser>();
		if (laser == nullptr) {
			laser = Instantiate<Laser>(GetParent());
			player->MyWeaponList_.push_back(laser);
		}
		else {
			laser->LevelUp(WeaponList_[_num].instruction_.front());
		}
	}
		break;
	case 5:
	{
		Bomb* bomb = GetParent()->FindGameObject<Bomb>();
		if (bomb == nullptr) {
			bomb = Instantiate<Bomb>(GetParent());
			player->MyWeaponList_.push_back(bomb);
		}
		else {
			bomb->LevelUp(WeaponList_[_num].instruction_.front());
		}
	}
		break;
	case 6:
		break;
	default:
		break;
	}

	WeaponList_[_num].instruction_.pop_back();
}


void HUD::TimerDraw()
{
	FontData data{};
	data.fontSize = 30;
	data.Color=D2D1::ColorF(255, 255, 255);
	data.font = TextFont::GetFontName(FontList::Gkktt);

	int Stime = fmodf(PlayTime_, 60.0f);
	int Mtime = PlayTime_ / 60;
	std::string minutes = std::to_string(Mtime);
	std::string seconds = std::to_string(Stime);
	minutes.insert(0, 2 - minutes.length(), '0');
	seconds.insert(0, 2 - seconds.length(), '0');
	TextFont::Draw(minutes + ":" + seconds.c_str(), { 600,30 }, data);
}