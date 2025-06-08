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
		const int RADARALPHA{ 200 };
	}
	namespace LEVEL {
		const float LEVELGAUGEBARXPOS{ -1.0f };
		const int WEAPONCHOICEVAL{ 4 };


		const std::string WEAPONNAME[WEAPONTYPE::END] = { "Knife","PoisonThrow","SpikeOrb","Missile","Laser","Bomb" };
		const float FRAMEIMAGEBUFFER{ 175 };
	}
}


void HUD::UIPosRead()
{
	enum CSVData
	{
		Type,
		Name,
		PosX,
		PosY,
		PosZ,
		RotX,
		RotY,
		RotZ,
		ScaX,
		ScaY,
		ScaZ,
	};

	CsvReader csvTrans;
	if (!csvTrans.Load("Assets\\CSV\\UI.csv"))
		return;

	for (int i = 1;i < csvTrans.GetHeight();i++) {
		//初期化
		HUDTransforms_[i - 1] = transform_;
		//読み込み
		HUDTransforms_[i - 1].position_ = { csvTrans.GetValue(CSVData::PosX,i),csvTrans.GetValue(CSVData::PosY,i) ,csvTrans.GetValue(CSVData::PosZ,i) };
		HUDTransforms_[i - 1].rotate_ = { csvTrans.GetValue(CSVData::RotX,i),csvTrans.GetValue(CSVData::RotY,i) ,csvTrans.GetValue(CSVData::RotZ,i) };
		HUDTransforms_[i - 1].scale_ = { csvTrans.GetValue(CSVData::ScaX,i),csvTrans.GetValue(CSVData::ScaY,i) ,csvTrans.GetValue(CSVData::ScaZ,i) };
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
	HPInitialize();
	UIPosRead();
}

void HUD::SuperUpdate()
{
	LevelSuperUpdate();
}

void HUD::Update()
{
	LevelUpdate();
	RadarUpdate();

	HPUpdate();
}

void HUD::Draw()
{
	TimerDraw();
	RadarDraw();
	LevelDraw();
	HPDraw();
}

void HUD::Release()
{
}

void HUD::RadarInitialize()
{
	hRadarBack_ = -1;
	hRadarFrame_ = -1;
	hREnemy_ = -1;
	hRPlayer_ = -1;


	hRadarBack_ = Image::Load("Assets\\Image\\UI\\Radar192-2Back.png");
	HandleCheck(hRadarBack_, "レーダーの背景の画像がない");
	hRadarFrame_ = Image::Load("Assets\\Image\\UI\\Radar192-5Frame.png");
	HandleCheck(hRadarFrame_, "レーダーのフレームの画像がない");
	hREnemy_ = Image::Load("Assets\\Image\\UI\\RadarEnemy.png");
	HandleCheck(hREnemy_, "レーダーの敵の画像がない");
	hRPlayer_ = Image::Load("Assets\\Image\\UI\\RadarPlayer.png");
	HandleCheck(hRPlayer_, "レーダーのプレイヤーの画像がない");

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
	HUDTransforms_[TRANSFORMTYPE::RADARPLAYER].rotate_.z = -player->GetRotate().y;
}

void HUD::RadarDraw()
{
	//透明度変更
	Image::SetAlpha(hREnemy_, RADAR::RADARALPHA);
	Image::SetAlpha(hRadarFrame_, RADAR::RADARALPHA);
	Image::SetAlpha(hRadarBack_, RADAR::RADARALPHA);
	Image::SetAlpha(hRadarBack_, 100);
	//レーダーの背景
	Image::SetTransform(hRadarBack_, HUDTransforms_[TRANSFORMTYPE::RADAR]);
	Image::Draw(hRadarBack_);
	Image::SetTransform(hRadarFrame_, HUDTransforms_[TRANSFORMTYPE::RADAR]);
	Image::Draw(hRadarFrame_);

	//レーダーのプレイヤー
	Image::SetTransform(hRPlayer_, HUDTransforms_[TRANSFORMTYPE::RADARPLAYER]);
	Image::Draw(hRPlayer_);

	//レーダーの敵
	for (auto I : REnemyPosList_) {
		Transform enemyTransform = HUDTransforms_[TRANSFORMTYPE::RADARENEMY];
		enemyTransform.position_ = I + enemyTransform.position_;
		Image::SetTransform(hREnemy_, enemyTransform);
		Image::Draw(hREnemy_);
	}

}

//レベルアップした時の処理
void HUD::LevelUP()
{

	Pause_ = true;

	//リストの整理
	for (auto itr = EquipmentList_.begin(); itr != EquipmentList_.end();) {
		WeaponObject* obj = (WeaponObject*)GetParent()->FindChildObject((*itr).name_);
		if (obj != nullptr) {
			if (obj->GetLv() >= (*itr).MaxLevel_) {
				itr = EquipmentList_.erase(itr);
				continue;
			}
		}
		itr++;
	}

	WeaponRoll();
}

void HUD::WeaponRoll()
{

	if (EquipmentList_.size() >= LEVEL::WEAPONCHOICEVAL) {
		while (true)
		{
			RollListNum_.clear();

			for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
				RollListNum_.insert(rand() % EquipmentList_.size());
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


	hLevelBack_ = Image::Load("Assets\\Image\\UI\\LevelUpBackGround0.6.png");
	HandleCheck(hLevelBack_);
	hLevelGaugeFrame_ = Image::Load("Assets\\Image\\UI\\LevelFrame.png");
	HandleCheck(hLevelGaugeFrame_);
	hLevelGaugeBar_ = Image::Load("Assets\\Image\\UI\\LevelBar.png");
	HandleCheck(hLevelGaugeBar_);
	hLevelCursor_ = Image::Load("Assets\\Image\\UI\\LevelCursor.png");
	HandleCheck(hLevelCursor_);
	hLevelFrameImage_ = Image::Load("Assets\\Image\\UI\\LevelUpFrame.png");
	HandleCheck(hLevelFrameImage_);
	hLevelCursorImage_ = Image::Load("Assets\\Image\\UI\\LevelUpCursor.png");
	HandleCheck(hLevelCursorImage_);


	for (int i = 0;i < WEAPONTYPE::END;i++) {
		hLevelIconImage_[i] = Image::Load("Assets\\Image\\Icon\\" +LEVEL::WEAPONNAME[i] + "_Icon.png");
		HandleCheck(hLevelIconImage_[i]);
	}

	CsvReader csv, effect;
	if (!csv.Load("Assets\\CSV\\WeaponList.csv"))
		return;
	if (!effect.Load("Assets\\CSV\\WeaponLevelText.csv"))
		return;

	for (int i = 1; i < csv.GetHeight(); i++) {
		EquipmentList_.push_back({ csv.GetString(0, i),(int)csv.GetValue(1, i), (int)csv.GetValue(2, i) });
		//pop_backができるので逆から入れてみる
		for (int j = EquipmentList_[i - 1].MaxLevel_ - 1 - 1; j >= 0; j--) {
			EquipmentList_[i - 1].instruction_.push_back(csv.GetString(3 + j, i));
		}
		for (int j = 2; j < effect.GetWidth(); j++) {
			EquipmentList_[i - 1].EffectText_[j - 2] = effect.GetString(j, i);
		}
	}


	StickTriggerY_ = false;
}

void HUD::LevelSuperUpdate()
{
	if (Pause_) {

		if ((Input::IsKeyDown(DIK_UP) || Input::GetPadStickL().y >= 0.5f) && !StickTriggerY_)
		{
			levelCursor_--;
			StickTriggerY_ = true;
		}
		else if ((Input::IsKeyDown(DIK_DOWN) || Input::GetPadStickL().y <= -0.5f) && !StickTriggerY_) {
			levelCursor_++;
			StickTriggerY_ = true;
		}
		if (Input::GetPadStickL().y >= -0.5f && Input::GetPadStickL().y <= 0.5f)
			StickTriggerY_ = false;

		levelCursor_ = levelCursor_ % LEVEL::WEAPONCHOICEVAL;
		if (levelCursor_ < 0)
			levelCursor_ = 3;

		Transform localTrans;
		localTrans.position_= { HUDTransforms_[TRANSFORMTYPE::LEVELCURSOR].position_.x,HUDTransforms_[TRANSFORMTYPE::LEVELCURSOR].position_.y - (levelCursor_ * LEVEL::FRAMEIMAGEBUFFER) / (screenHeight / 2.0f),0 };
		Image::SetTransform(hLevelCursorImage_, localTrans);

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

	HUDTransforms_[TRANSFORMTYPE::LEVELGAUGEBAR].scale_ = { ratio,1,1 };
}

void HUD::LevelDraw()
{
	Image::SetTransform(hLevelGaugeFrame_, HUDTransforms_[TRANSFORMTYPE::LEVELGAUGEFRAME]);
	Image::Draw(hLevelGaugeFrame_);

	Image::SetTransform(hLevelGaugeBar_, HUDTransforms_[TRANSFORMTYPE::LEVELGAUGEBAR]);
	Image::Draw(hLevelGaugeBar_);

	if (Pause_) {
		Image::SetTransform(hLevelBack_, HUDTransforms_[TRANSFORMTYPE::LEVELBACK]);
		Image::Draw(hLevelBack_);

		//先にTransformのセットはやってある
		Image::Draw(hLevelCursorImage_);

		Player* player = GetParent()->FindGameObject<Player>();

		//レベルアップで選択できる武器
		auto itr = RollListNum_.begin();
		for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
			//														初期値-画像の縦の長さ/2+i*画像の長さ+バッファ
			Transform FrameTrans;
			FrameTrans.position_ = { HUDTransforms_[TRANSFORMTYPE::LEVELFRAME].position_.x,HUDTransforms_[TRANSFORMTYPE::LEVELFRAME].position_.y - (i * LEVEL::FRAMEIMAGEBUFFER) / (screenHeight / 2.0f),0 };
			Image::SetTransform(hLevelFrameImage_, FrameTrans);
			Image::Draw(hLevelFrameImage_);

			std::string level = "new";
			int Lv = 0;
			FontData data;
			data.font = TextFont::GetFontName(FontList::Makinas);
			data.Color = D2D1::ColorF(255, 255, 255);
			data.fontSize = 30;

			//武器の名前
			TextFont::Draw(EquipmentList_[(*std::next(itr, i))].name_.c_str(), { 780,40.0f + (i * (170 + 5)) }, data);

			//武器のレベル
			for (int j = 0; j < player->MyWeaponList_.size(); j++) {
				if (EquipmentList_[(*std::next(itr, i))].name_ == player->MyWeaponList_[j]->GetObjectName()) {
					level = "Level:" + std::to_string(player->MyWeaponList_[j]->GetLv() + 1);
					Lv = player->MyWeaponList_[j]->GetLv();
					break;
				}
			}

			//アイコンを表示
			for (int j = 0;j < WEAPONTYPE::END;j++) {
				if (LEVEL::WEAPONNAME[j] == EquipmentList_[(*std::next(itr, i))].name_) {
					//----------------------------------------------------------------------------------------------
					Transform IconTrans;
					IconTrans.position_ = { HUDTransforms_[TRANSFORMTYPE::LEVELICON].position_.x,HUDTransforms_[TRANSFORMTYPE::LEVELICON].position_.y - (i * LEVEL::FRAMEIMAGEBUFFER) / (screenHeight / 2.0f),0 };
					Image::SetTransform(hLevelIconImage_[j], IconTrans);
					Image::Draw(hLevelIconImage_[j]);
				}
			}

			TextFont::Draw(EquipmentList_[(*std::next(itr, i))].EffectText_[Lv].c_str(), { 780,105.0f + (i * 170) }, { 1250,115.0f + (i * 170) }, data);

			TextFont::Draw(level.c_str(), { 1100,40.0f + (i * (170 + 5)) }, data);

		}
	}
}

void HUD::ObtainWeapon(int _num)
{
	Player* player = GetParent()->FindGameObject<Player>();
	if (player == nullptr)
		return;


	switch (EquipmentList_[_num].num_)
	{
	case WEAPONTYPE::KNIFE: {
		Knife* knife = GetParent()->FindGameObject<Knife>();
		if (knife == nullptr) {
			knife = Instantiate<Knife>(GetParent());
			player->MyWeaponList_.push_back(knife);
		}
		else {
			knife->LevelUp(EquipmentList_[_num].instruction_.back());
		}
	}
						  break;
	case WEAPONTYPE::POISONTHROW: {
		PoisonThrow* poison = GetParent()->FindGameObject<PoisonThrow>();
		if (poison == nullptr) {
			poison = Instantiate<PoisonThrow>(GetParent());
			player->MyWeaponList_.push_back(poison);
		}
		else {
			poison->LevelUp(EquipmentList_[_num].instruction_.front());
		}
	}
								break;
	case WEAPONTYPE::SPIKEORB:
	{
		SpikeOrb* spike = GetParent()->FindGameObject<SpikeOrb>();
		if (spike == nullptr) {
			spike = Instantiate<SpikeOrb>(GetParent());
			player->MyWeaponList_.push_back(spike);
		}
		else {
			spike->LevelUp(EquipmentList_[_num].instruction_.front());
		}
	}
	break;
	case WEAPONTYPE::MISSILE:
	{
		Missile* missile = GetParent()->FindGameObject<Missile>();
		if (missile == nullptr) {
			missile = Instantiate<Missile>(GetParent());
			player->MyWeaponList_.push_back(missile);
		}
		else {
			missile->LevelUp(EquipmentList_[_num].instruction_.front());
		}
	}
	break;
	case WEAPONTYPE::LASER:
	{
		Laser* laser = GetParent()->FindGameObject<Laser>();
		if (laser == nullptr) {
			laser = Instantiate<Laser>(GetParent());
			player->MyWeaponList_.push_back(laser);
		}
		else {
			laser->LevelUp(EquipmentList_[_num].instruction_.front());
		}
	}
	break;
	case WEAPONTYPE::BOMB:
	{
		Bomb* bomb = GetParent()->FindGameObject<Bomb>();
		if (bomb == nullptr) {
			bomb = Instantiate<Bomb>(GetParent());
			player->MyWeaponList_.push_back(bomb);
		}
		else {
			bomb->LevelUp(EquipmentList_[_num].instruction_.front());
		}
	}
	break;
	case 6:
		break;
	default:
		break;
	}

	EquipmentList_[_num].instruction_.pop_back();
}

void HUD::TimerDraw()
{
	FontData data{};
	data.fontSize = 30;
	data.Color = D2D1::ColorF(255, 255, 255);
	data.font = TextFont::GetFontName(FontList::Gkktt);

	int Stime = fmodf(PlayTime_, 60.0f);
	int Mtime = PlayTime_ / 60;
	std::string minutes = std::to_string(Mtime);
	std::string seconds = std::to_string(Stime);
	minutes.insert(0, 2 - minutes.length(), '0');
	seconds.insert(0, 2 - seconds.length(), '0');
	TextFont::Draw(minutes + ":" + seconds.c_str(), { 600,30 }, data);
}

void HUD::HPInitialize()
{
	hHPBack_ = Image::Load("Assets\\Image\\UI\\HPBack.png");
	HandleCheck(hHPBack_);
	hHPGauge_ = Image::Load("Assets\\Image\\UI\\HP.png");
	HandleCheck(hHPGauge_);
	hHPFrame_ = Image::Load("Assets\\Image\\UI\\HPFrame.png");
	HandleCheck(hHPFrame_);
	hHPIcon_ = Image::Load("Assets\\Image\\UI\\Heart.png");
	HandleCheck(hHPIcon_);
	hHPFull_ = Image::Load("Assets\\Image\\UI\\FullHP.png");
	HandleCheck(hHPFull_);

}

void HUD::HPUpdate()
{
	Player* player = GetParent()->FindGameObject<Player>();
	NullCheck(player);
	//HPの大きさを変える
	float ratio = player->GetStatus().hp_ / (float)player->GetStatus().maxHp_;
	HUDTransforms_[TRANSFORMTYPE::HPBACK].scale_ = { 1 - ratio,1,1 };
}

void HUD::HPDraw()
{

	Player* player = GetParent()->FindGameObject<Player>();
	NullCheck(player);

	Image::SetTransform(hHPIcon_, HUDTransforms_[TRANSFORMTYPE::HPICON]);
	Image::Draw(hHPIcon_);

	if (player->GetStatus().hp_ == player->GetStatus().maxHp_) {
		Image::SetTransform(hHPFull_, HUDTransforms_[TRANSFORMTYPE::HPGAUGE]);
		Image::Draw(hHPFull_);
	}
	else {
		Image::SetTransform(hHPGauge_, HUDTransforms_[TRANSFORMTYPE::HPGAUGE]);
		Image::Draw(hHPGauge_);
	}

	Image::SetTransform(hHPBack_, HUDTransforms_[TRANSFORMTYPE::HPBACK]);
	Image::Draw(hHPBack_);
	Image::SetTransform(hHPFrame_, HUDTransforms_[TRANSFORMTYPE::HPFRAME]);
	Image::Draw(hHPFrame_);

	FontData data;
	data.font = TextFont::GetFontName(FontList::Gkktt);
	data.Color = D2D1::ColorF(0, 0, 0);
	data.fontSize = 20;
	//HPの大きさを変える
	TextFont::Draw(std::to_string(player->GetStatus().hp_) + "/" + std::to_string(player->GetStatus().maxHp_), { 130,23 }, data);
}
