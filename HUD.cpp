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

void HUD::LevelUP()
{
	Pause_ = true;

	LGBarTransform_.scale_ = { 1,1,1 };

	//リストを調べる
	for (auto I = WeaponList_.begin(); I != WeaponList_.end(); I++) {

		GameObject* obj = GetParent()->FindChildObject((*I).second);
		if (obj != nullptr) {
			I = WeaponList_.erase(I);
		}
		else
			I++;

	}

	//選べる武器を選択(まだ未完成)
	if (WeaponList_.size() >= LEVEL::WEAPONCHOICEVAL) {
		while (true)
		{
			choiceWeapon_.clear();
			for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
				choiceWeapon_.insert(rand() % WeaponList_.size());
			}
			if (choiceWeapon_.size() == LEVEL::WEAPONCHOICEVAL)
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

	LGFrameTransform_.position_ = { 0,LEVEL::LEVELGAUGEYPOS,0 };
	LGBarTransform_.position_ = { LEVEL::LEVELGAUGEBARXPOS,LEVEL::LEVELGAUGEYPOS,0 };

	hLevelBack_ = Image::Load("Assets\\Image\\LevelUpBackGround0.6.png");
	assert(hLevelBack_ >= 0);
	hLevelGaugeFrame_ = Image::Load("Assets\\Image\\LevelFrame.png");
	assert(hLevelGaugeFrame_ >= 0);
	hLevelGaugeBar_ = Image::Load("Assets\\Image\\LevelBar.png");
	assert(hLevelGaugeBar_ >= 0);

	WeaponList_.clear();

	//武器リストを作成
	CsvReader csv;
	csv.Load("Assets\\CSV\\WeaponList.csv");
	for (int i = 1; i < csv.GetHeight(); i++) {
		std::string str = csv.GetString(0, i);
		GameObject* obj = GetParent()->FindChildObject(str);
		if (obj == nullptr)
			WeaponList_.push_back({ csv.GetValue(1,i),csv.GetString(0,i) });

	}
}

void HUD::LevelSuperUpdate()
{


	if (Pause_) {
		if (Input::IsKeyDown(DIK_UP))
			choice_--;
		if (Input::IsKeyDown(DIK_DOWN))
			choice_++;

		choice_ = choice_ % LEVEL::WEAPONCHOICEVAL;
		if (choice_ < 0)
			choice_ = 3;

		auto itr = choiceWeapon_.begin();
		std::advance(itr, choice_);
		int num = WeaponList_[(*itr)].first;

		if (Input::IsKeyDown(DIK_RETURN)) {

			ObtainWeapon(num);
		}
	}
}

void HUD::LevelUpdate()
{
	Pause_ = false;
	choice_ = 0;

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

		int count = 1;
		for (auto I : choiceWeapon_) {
			ptext_->Draw(900, 30 + (count * 50), WeaponList_[I].second.c_str());
			count++;
		}
	}
}

void HUD::ObtainWeapon(int _num)
{
	Player* player = GetParent()->FindGameObject<Player>();
	Debug::Log(_num);
	switch (_num)
	{
	case 0: {
		Knife* knife = Instantiate<Knife>(GetParent());
		player->MyWeaponList_.push_back(knife);
		break;
	}
	case 1: {
		PoisonThrow* poison = Instantiate<PoisonThrow>(GetParent());
		player->MyWeaponList_.push_back(poison);
		break;
	}
	default:
		break;
	}
}
