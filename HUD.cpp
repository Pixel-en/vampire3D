#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"

#include "Player.h"
#include "EnemySpawn.h"
#include "Knife.h"
#include "PoisonThrow.h"

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

	ptext_ = new Text;
	ptext_->Initialize();
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
	RadarDraw();
	LevelDraw();
}

void HUD::Release()
{
	ptext_->Release();
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

	LBackTransform_ = transform_;
	LGFrameTransform_ = transform_;
	LGBarTransform_ = transform_;
	LCursorTransform_ = transform_;

	LGFrameTransform_.position_ = { 0,LEVEL::LEVELGAUGEYPOS,0 };
	LGBarTransform_.position_ = { LEVEL::LEVELGAUGEBARXPOS,LEVEL::LEVELGAUGEYPOS,0 };

	hLevelBack_ = Image::Load("Assets\\Image\\LevelUpBackGround0.6.png");
	assert(hLevelBack_ >= 0);
	hLevelGaugeFrame_ = Image::Load("Assets\\Image\\LevelFrame.png");
	assert(hLevelGaugeFrame_ >= 0);
	hLevelGaugeBar_ = Image::Load("Assets\\Image\\LevelBar.png");
	assert(hLevelGaugeBar_ >= 0);
	hLevelCursor_ = Image::Load("Assets\\Image\\LevelCursor.png");
	assert(hLevelCursor_ >= 0);

	CsvReader csv;
	csv.Load("Assets\\CSV\\WeaponList.csv");

	for (int i = 1; i < csv.GetHeight(); i++) {
		WeaponList_.push_back({ csv.GetString(0, i),csv.GetValue(1, i), csv.GetValue(2, i) });
		//pop_backができるので逆から入れてみる
		for (int j = WeaponList_[i - 1].MaxLevel_ - 1 - 1; j >= 0; j--) {
			WeaponList_[i - 1].instruction_.push_back(csv.GetString(3 + j, i));
		}
	}
}

void HUD::LevelSuperUpdate()
{


	if (Pause_) {

		if (Input::IsKeyDown(DIK_UP))
			levelCursor_--;
		else if (Input::IsKeyDown(DIK_DOWN))
			levelCursor_++;

		levelCursor_ = levelCursor_ % LEVEL::WEAPONCHOICEVAL;
		if (levelCursor_ < 0)
			levelCursor_ = 3;

		LCursorTransform_.position_.y = 0.6 + -0.1 * levelCursor_;

		if (Input::IsKeyDown(DIK_RETURN)) {
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
	float current = player->GetCurrentExp();
	float next = player->GetnextLvExp();

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
		
		Image::SetTransform(hLevelCursor_, LCursorTransform_);
		Image::Draw(hLevelCursor_);

		auto itr = RollListNum_.begin();
		for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
			ptext_->Draw(900, 150 + (i * 50), WeaponList_[ (*std::next(itr, i)) ].name_.c_str());
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
			knife->LevelUp(WeaponList_[_num].instruction_.front());
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
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}

	WeaponList_[_num].instruction_.pop_back();
}
