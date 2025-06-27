#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/TextFont.h"
#include <array>

#include "Player.h"
#include "EnemySpawn.h"

#include "Knife.h"
#include "PoisonThrow.h"
#include "SpikeOrb.h"
#include "Missile.h"
#include "Laser.h"
#include "Bomb.h"
#include "Armor.h"
#include "FreeMind.h"
#include "WideAmulet.h"
#include "KnowledgeBook.h"
#include "HeartCream.h"
#include "WonderCookie.h"
#include "MuscleSuit.h"
#include "EnergyDrink.h"
#include "MagicHand.h"
#include "Monocle.h"
#include "Cushion.h"
#include "LifeFragment.h"

namespace {
	namespace RADAR {
		const float RADARSCALE{ 1000.0f };			//レーダーのサイズに縮小
		const float RADARRANGE{ 14.0f / 100.0f };	//レーダーの感知範囲
		const int RADARALPHA{ 200 };
	}
	namespace LEVEL {
		const float LEVELGAUGEBARXPOS{ -1.0f };
		const int WEAPONCHOICEVAL{ 4 };

		const int EQUIPMENTLISTNUM = WEAPONTYPE::END + WEAPONTYPE::AEND - WEAPONTYPE::ARMOR + WEAPONTYPE::SEND - WEAPONTYPE::MAXHP;

		//装備の名前リスト
		const std::array<std::string,  EQUIPMENTLISTNUM>EQUIPMENTSNAMELIST = {
			"Knife","PoisonThrow","SpikeOrb","Missile","Laser","Bomb",
			"Armor","FreeMind","WideAmulet","KnowledgeBook","HeartCream",
			"WonderCookie","MuscleSuit","EnergyDrink","MagicHand","Monocle",
			"Cushion","LifeFragment","MaxHp","Speed","Strength","Critical","Collect","Haste"
		};
		const float FRAMEIMAGEBUFFER{ 175 };
		const int EQUIPMENTSMAX{ 6 };
	}
	namespace EQUIPMENTS {
		const float ICONBUFFERWIDTH{ 0.065f };
		const float ICONBUFFERHEIGHT{ 0.12f };
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

	for (int i = 1; i < csvTrans.GetHeight(); i++) {
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
	EquipmentInitialize();
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
	EquipmentDraw();
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
	if (Pause_) {
		LevelUpdate();
	}

	Pause_ = true;

	Player* player = GetParent()->FindGameObject<Player>();

	//装備数が最大になったら
	if (player->MyWeaponList_.size() >= LEVEL::EQUIPMENTSMAX) {
		for (auto itr = EquipmentList_.begin(); itr != EquipmentList_.end();) {
			//武器
			if ((*itr).num_ <= WEAPONTYPE::END) {
				WeaponObject* obj = (WeaponObject*)GetParent()->FindChildObject((*itr).name_);
				if (obj == nullptr) {
					itr = EquipmentList_.erase(itr);
					continue;
				}
			}
			itr++;
		}
	}

	if (player->MyArmorList_.size() >= LEVEL::EQUIPMENTSMAX) {
		for (auto itr = EquipmentList_.begin(); itr != EquipmentList_.end();) {
			//防具
			if ((*itr).num_ > WEAPONTYPE::END && (*itr).num_ < WEAPONTYPE::AEND) {
				ArmorObject* armor = (ArmorObject*)GetParent()->FindChildObject((*itr).name_);
				if (armor == nullptr) {
					itr = EquipmentList_.erase(itr);
					continue;
				}
			}
			itr++;
		}
	}

	//リストの整理
	//レベルが最大のものがあればリストから除外
	for (auto itr = EquipmentList_.begin(); itr != EquipmentList_.end();) {
		if ((*itr).num_ <= WEAPONTYPE::END) {
			WeaponObject* obj = (WeaponObject*)GetParent()->FindChildObject((*itr).name_);
			if (obj != nullptr) {
				if (obj->GetLv() >= (*itr).MaxLevel_) {
					itr = EquipmentList_.erase(itr);
					continue;
				}
			}
		}
		else {
			ArmorObject* armor = (ArmorObject*)GetParent()->FindChildObject((*itr).name_);
			if (armor != nullptr) {
				if (armor->GetNumLevel() >= (*itr).MaxLevel_) {
					itr = EquipmentList_.erase(itr);
					continue;
				}
			}
		}
		itr++;
	}

	WeaponRoll();
}

void HUD::WeaponRoll()
{
	//足りなくなったらステータスアップを追加する
	if (EquipmentList_.size() < LEVEL::WEAPONCHOICEVAL) {
		EquipmentList_.insert(EquipmentList_.end(), StatusUpList_.begin(), StatusUpList_.end());
	}

	//選ぶ
	while (true)
	{
		RollListNum_.clear();
		//ランダムに選び重複していないならループから抜ける
		for (int i = 0; i < LEVEL::WEAPONCHOICEVAL; i++) {
			RollListNum_.insert(rand() % EquipmentList_.size());
		}
		if (RollListNum_.size() == LEVEL::WEAPONCHOICEVAL)
			break;
	}

}

void HUD::LevelInitialize()
{
	hLevelBack_ = -1;
	hLevelGaugeFrame_ = -1;
	hLevelGaugeBar_ = -1;
	hLevelFrameImage_ = -1;
	hLevelCursorImage_ = -1;
	isChoice_ = false;


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

	enum CSVDATA {
		NAME,
		DISPLAYNAME,
		NUM,
		MAXLEVEL,
		INSTRUCTION1,
		EFFECT1,
	};

	CsvReader csv, effect, armors, Atext;
	if (!csv.Load("Assets\\CSV\\WeaponList.csv"))
		return;
	if (!effect.Load("Assets\\CSV\\WeaponLevelText.csv"))
		return;
	if (!armors.Load("Assets\\CSV\\ArmorsList.csv"))
		return;
	if (!Atext.Load("Assets\\CSV\\ArmorsLevelText.csv"))
		return;

	//装備リストに入れる
	for (int i = 1; i < csv.GetHeight(); i++) {
		//装備の名前、装備の番号、装備の最大レベルを取得する
		EquipmentList_.push_back({ csv.GetString(NAME, i),csv.GetString(DISPLAYNAME,i),(int)csv.GetValue(NUM, i), (int)csv.GetValue(MAXLEVEL, i) });
		//レベルアップ内容などを取得する
		//pop_backができるので逆から入れてみる		レベル2スタートなので-1，配列とCSVは１ずれるので-1
		for (int j = EquipmentList_[i - 1].MaxLevel_ - 1 - 1; j >= 0; j--) {
			EquipmentList_[i - 1].instruction_.push_back(csv.GetString(4 + j, i));
		}
		for (int j = 2; j < effect.GetWidth(); j++) {
			EquipmentList_[i - 1].EffectText_[j - 2] = effect.GetString(j, i);
		}
	}

	for (int i = 1; i < armors.GetHeight(); i++) {
		//装備の名前、装備の番号、装備の最大レベルを取得する
		EquipmentList_.push_back({ armors.GetString(NAME, i),armors.GetString(DISPLAYNAME,i),(int)armors.GetValue(NUM, i), (int)armors.GetValue(MAXLEVEL, i) });
		//レベルアップ内容などを取得する
		//pop_backができるので逆から入れてみる
		for (int j = EquipmentList_[WEAPONTYPE::END + i - 1].MaxLevel_ - 1; j >= 0; j--) {
			EquipmentList_[WEAPONTYPE::END + i - 1].instruction_.push_back(armors.GetString(4 + j, i));
		}
		for (int j = 2; j < Atext.GetWidth(); j++) {
			EquipmentList_[WEAPONTYPE::END + i - 1].EffectText_[j - 2] = Atext.GetString(j, i);
		}
	}

	//ここでインスタンスを読んでいるわけではないのであると仮定して作る
	CsvReader SUp;
	if (!SUp.Load("Assets\\CSV\\StatusList.csv"))
		return;

	//ステータスアップの装備を追加
	for (int i = 1; i < SUp.GetHeight(); i++) {
		StatusUpList_.push_back({ SUp.GetString(NAME, i),SUp.GetString(DISPLAYNAME,i),(int)SUp.GetValue(NUM, i), (int)SUp.GetValue(MAXLEVEL, i) });
		StatusUpList_[i - 1].instruction_.push_back(SUp.GetString(INSTRUCTION1, i));	//レベルアップ内容は一つだけ
		StatusUpList_[i - 1].EffectText_[0] = SUp.GetString(EFFECT1, i);	//効果説明は一つだけ
	}

	//アイコン画像のロード
	for (int i = 0; i < LEVEL::EQUIPMENTLISTNUM; i++) {
		//装備のリストの数だけアイコンをロード
		if (i < EquipmentList_.size()) {
			hLevelIconImage_[i] = Image::Load("Assets\\Image\\Icon\\" + EquipmentList_[i].name_ + "_Icon.png");
		}
		//ステータスアップの数だけアイコンをロード
		else {
			hLevelIconImage_[i] = Image::Load("Assets\\Image\\Icon\\" + StatusUpList_[i - EquipmentList_.size()].name_ + "_Icon.png");
		}
		HandleCheck(hLevelIconImage_[i]);
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
		localTrans.position_ = { HUDTransforms_[TRANSFORMTYPE::LEVELCURSOR].position_.x,HUDTransforms_[TRANSFORMTYPE::LEVELCURSOR].position_.y - (levelCursor_ * LEVEL::FRAMEIMAGEBUFFER) / (screenHeight / 2.0f),0 };
		Image::SetTransform(hLevelCursorImage_, localTrans);

		if (Input::IsKeyDown(DIK_RETURN) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B)) {
			isChoice_ = true;
			auto itr = RollListNum_.begin();
			std::advance(itr, levelCursor_);
			ObtainWeapon((*itr));
		}
	}
}

void HUD::LevelUpdate()
{
	isChoice_ = false;
	Pause_ = false;

	Player* player = GetParent()->FindGameObject<Player>();
	levelCursor_ = 0;

	float current = player->GetStatus().currentExp_;
	float next = player->GetStatus().nextLvExp_;

	float ratio = current / next;

	HUDTransforms_[TRANSFORMTYPE::LEVELGAUGEBAR].scale_ = { ratio,1,1 };



}

void HUD::LevelDraw()
{


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
			data.fontSize = 27;

			//武器の名前
			TextFont::Draw(EquipmentList_[(*std::next(itr, i))].displayName_.c_str(), { 780,40.0f + (i * (170 + 5)) }, data);


			//武器のレベル
			//武器のリストになかったら装備のリストに
			for (int j = 0; j < player->MyWeaponList_.size(); j++) {
				if (EquipmentList_[(*std::next(itr, i))].name_ == player->MyWeaponList_[j]->GetObjectName()) {
					level = "Level:" + std::to_string(player->MyWeaponList_[j]->GetLv() + 1);
					Lv = player->MyWeaponList_[j]->GetLv();
					break;
				}
			}

			for (int j = 0; j < player->MyArmorList_.size(); j++) {
				if (EquipmentList_[(*std::next(itr, i))].name_ == player->MyArmorList_[j]->GetObjectName()) {
					level = "Level:" + std::to_string(player->MyArmorList_[j]->GetNumLevel() + 1);
					Lv = player->MyArmorList_[j]->GetNumLevel();
					break;
				}
			}

			//それでもだめならステータスのリストに
			for (int j = 0; j < StatusUpList_.size(); j++) {
				if (EquipmentList_[(*std::next(itr, i))].name_ == StatusUpList_[j].name_) {
					level = "";
					Lv = 0;	//ステータスアップはレベルがないので0
					break;
				}
			}

			//アイコンを表示
			for (int j = 0; j < LEVEL::EQUIPMENTSNAMELIST.size(); j++) {
				//出現できる武器のリストは短くなっていてそのままだと、画像のハンドルのインデックスとは合わなくなっているので
				//装備の名前と装備リストの名前を比較して同じならその番号が使える
				if (LEVEL::EQUIPMENTSNAMELIST[j] == EquipmentList_[(*std::next(itr, i))].name_) {
					//----------------------------------------------------------------------------------------------
					Transform IconTrans;
					IconTrans.position_ = { HUDTransforms_[TRANSFORMTYPE::LEVELICON].position_.x,HUDTransforms_[TRANSFORMTYPE::LEVELICON].position_.y - (i * LEVEL::FRAMEIMAGEBUFFER) / (screenHeight / 2.0f),0 };
					Image::SetTransform(hLevelIconImage_[j], IconTrans);
					Image::Draw(hLevelIconImage_[j]);
				}
			}

			//説明
			TextFont::Draw(EquipmentList_[(*std::next(itr, i))].EffectText_[Lv].c_str(), { 780,105.0f + (i * 170) }, { 1250,115.0f + (i * 170) }, data);

			//レベル
			TextFont::Draw(level.c_str(), { 1100,40.0f + (i * (170 + 5)) }, data);

		}
	}

	//レベルゲージを表示
	Image::SetTransform(hLevelGaugeFrame_, HUDTransforms_[TRANSFORMTYPE::LEVELGAUGEFRAME]);
	Image::Draw(hLevelGaugeFrame_);

	Image::SetTransform(hLevelGaugeBar_, HUDTransforms_[TRANSFORMTYPE::LEVELGAUGEBAR]);
	Image::Draw(hLevelGaugeBar_);
}

void HUD::ObtainWeapon(int _num)
{
	Player* player = GetParent()->FindGameObject<Player>();
	if (player == nullptr)
		return;

	bool needpop = true;

	switch (EquipmentList_[_num].num_)
	{
	case WEAPONTYPE::KNIFE: {
		Knife* knife = GetParent()->FindGameObject<Knife>();
		if (knife == nullptr) {
			knife = Instantiate<Knife>(GetParent());
			player->MyWeaponList_.push_back(knife);
			needpop = false;
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
			needpop = false;
		}
		else {
			poison->LevelUp(EquipmentList_[_num].instruction_.back());
		}
	}
								break;
	case WEAPONTYPE::SPIKEORB:
	{
		SpikeOrb* spike = GetParent()->FindGameObject<SpikeOrb>();
		if (spike == nullptr) {
			spike = Instantiate<SpikeOrb>(GetParent());
			player->MyWeaponList_.push_back(spike);
			needpop = false;
		}
		else {
			spike->LevelUp(EquipmentList_[_num].instruction_.back());
		}
	}
	break;
	case WEAPONTYPE::MISSILE:
	{
		Missile* missile = GetParent()->FindGameObject<Missile>();
		if (missile == nullptr) {
			missile = Instantiate<Missile>(GetParent());
			player->MyWeaponList_.push_back(missile);
			needpop = false;
		}
		else {
			missile->LevelUp(EquipmentList_[_num].instruction_.back());
		}
	}
	break;
	case WEAPONTYPE::LASER:
	{
		Laser* laser = GetParent()->FindGameObject<Laser>();
		if (laser == nullptr) {
			laser = Instantiate<Laser>(GetParent());
			player->MyWeaponList_.push_back(laser);
			needpop = false;
		}
		else {
			laser->LevelUp(EquipmentList_[_num].instruction_.back());
		}
	}
	break;
	case WEAPONTYPE::BOMB:
	{
		Bomb* bomb = GetParent()->FindGameObject<Bomb>();
		if (bomb == nullptr) {
			bomb = Instantiate<Bomb>(GetParent());
			player->MyWeaponList_.push_back(bomb);
			needpop = false;
		}
		else {
			bomb->LevelUp(EquipmentList_[_num].instruction_.back());
		}
	}
	break;

	case 6:
		break;

	case WEAPONTYPE::ARMOR: {
		Armor* armor = GetParent()->FindGameObject<Armor>();
		if (armor == nullptr) {
			armor = Instantiate<Armor>(GetParent());
			//ここあとで書いて
			player->MyArmorList_.push_back(armor);
		}

		armor->LevelUp(EquipmentList_[_num].instruction_.back());
	}
						  break;
	case WEAPONTYPE::FREEMIND: {
		FreeMind* critical = GetParent()->FindGameObject<FreeMind>();
		if (critical == nullptr) {
			critical = Instantiate<FreeMind>(GetParent());
			player->MyArmorList_.push_back(critical);
		}

		critical->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							 break;
	case WEAPONTYPE::WIDEAMULET: {
		WideAmulet* wide = GetParent()->FindGameObject<WideAmulet>();
		if (wide == nullptr) {
			wide = Instantiate<WideAmulet>(GetParent());
			player->MyArmorList_.push_back(wide);
		}
		wide->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							   break;
	case WEAPONTYPE::KNOWLEDGEBOOK:
	{
		KnowledgeBook* knowledge = GetParent()->FindGameObject<KnowledgeBook>();
		if (knowledge == nullptr) {
			knowledge = Instantiate<KnowledgeBook>(GetParent());
			player->MyArmorList_.push_back(knowledge);
		}
		knowledge->LevelUp(EquipmentList_[_num].instruction_.back());
	}
	break;
	case WEAPONTYPE::HEARTCREAM: {
		HeartCream* heart = GetParent()->FindGameObject<HeartCream>();
		if (heart == nullptr) {
			heart = Instantiate<HeartCream>(GetParent());
			player->MyArmorList_.push_back(heart);
		}
		heart->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							   break;
	case WEAPONTYPE::WONDERCOOKIE: {
		WonderCookie* wonder = GetParent()->FindGameObject<WonderCookie>();
		if (wonder == nullptr) {
			wonder = Instantiate<WonderCookie>(GetParent());
			player->MyArmorList_.push_back(wonder);
		}
		wonder->LevelUp(EquipmentList_[_num].instruction_.back());
	}
								 break;
	case WEAPONTYPE::MUSCLESUIT: {
		MuscleSuit* muscle = GetParent()->FindGameObject<MuscleSuit>();
		if (muscle == nullptr) {
			muscle = Instantiate<MuscleSuit>(GetParent());
			player->MyArmorList_.push_back(muscle);
		}
		muscle->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							   break;
	case WEAPONTYPE::ENERGYDRINK: {
		EnergyDrink* energy = GetParent()->FindGameObject<EnergyDrink>();
		if (energy == nullptr) {
			energy = Instantiate<EnergyDrink>(GetParent());
			player->MyArmorList_.push_back(energy);
		}
		energy->LevelUp(EquipmentList_[_num].instruction_.back());
	}
								break;
	case WEAPONTYPE::MAGICHAND: {
		MagicHand* magic = GetParent()->FindGameObject<MagicHand>();
		if (magic == nullptr) {
			magic = Instantiate<MagicHand>(GetParent());
			player->MyArmorList_.push_back(magic);
		}
		magic->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							  break;
	case WEAPONTYPE::MONOCLE: {
		Monocle* mono = GetParent()->FindGameObject<Monocle>();
		if (mono == nullptr) {
			mono = Instantiate<Monocle>(GetParent());
			player->MyArmorList_.push_back(mono);
		}
		mono->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							break;
	case WEAPONTYPE::CUSHION: {
		Cushion* cushion = GetParent()->FindGameObject<Cushion>();
		if (cushion == nullptr) {
			cushion = Instantiate<Cushion>(GetParent());
			player->MyArmorList_.push_back(cushion);
		}
		cushion->LevelUp(EquipmentList_[_num].instruction_.back());
	}
							break;
	case WEAPONTYPE::LIFEFRAGMENT: {
		LifeFragment* life = GetParent()->FindGameObject<LifeFragment>();
		if (life == nullptr) {
			life = Instantiate<LifeFragment>(GetParent());
			player->MyArmorList_.push_back(life);
		}
		life->LevelUp(EquipmentList_[_num].instruction_.back());
	}
								 break;
	case WEAPONTYPE::MAXHP: {
		//HPを増やす
		float temp = std::stof(EquipmentList_[_num].instruction_.back());
		player->AddStatusMaxHp(player->GetStatus().maxHp_ * temp);
		float uphp = player->GetStatus().maxHp_ * temp;
		player->StatusUpdate();	//一度ステータスの更新をすることで最大体力を増やす
		player->HealingHp(uphp);	//HPも回復する
		needpop = false;	//ポップしない
	}
						  break;
	case WEAPONTYPE::SPD: {
		//スピードを上げる
		float temp = std::stof(EquipmentList_[_num].instruction_.back());
		player->MultDivBoostStatusSpeed(temp, true);
		needpop = false;	//ポップしない
	}
						break;
	case WEAPONTYPE::STR: {
		//攻撃力を上げる
		float temp = std::stof(EquipmentList_[_num].instruction_.back());
		player->MultDivBoostStatusStrength(temp, true);
		needpop = false;	//ポップしない
	}
						break;
	case WEAPONTYPE::CRT: {
		//クリティカルを上げる
		float temp = std::stof(EquipmentList_[_num].instruction_.back());
		player->MultDivBoostStatusCritical(temp, true);
		needpop = false;	//ポップしない
	}
						break;
	case WEAPONTYPE::COLLECT: {
		//コレクトを上げる
		float temp = std::stof(EquipmentList_[_num].instruction_.back());
		player->MultDivBoostStatusCollectionRange(temp, true);
		needpop = false;	//ポップしない
	}
							break;
	case WEAPONTYPE::HASTE: {
		//ヘイストを上げる
		float temp = std::stof(EquipmentList_[_num].instruction_.back());
		player->MultDivBoostStatusHaste(temp, true);
		needpop = false;	//ポップしない
	}
						  break;
	default:
		break;
	}

	if (needpop)
		EquipmentList_[_num].instruction_.pop_back();

	player->StatusUpdate();
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


void HUD::EquipmentInitialize()
{
	hEquiIcon_ = Image::Load("Assets\\Image\\UI\\Hammer.png");
	HandleCheck(hEquiIcon_);
	hEquiBack_ = Image::Load("Assets\\Image\\UI\\EquipmentBack.png");
	HandleCheck(hEquiBack_);
}

void HUD::EquipmentDraw()
{

	Image::SetTransform(hEquiIcon_, HUDTransforms_[TRANSFORMTYPE::EQUIPICON]);
	Image::Draw(hEquiIcon_);
	Image::SetTransform(hEquiBack_, HUDTransforms_[TRANSFORMTYPE::EQUIPBACK]);
	Image::Draw(hEquiBack_);

	//アイコンを表示
	Player* player = GetRootJob()->FindGameObject<Player>();

	//武器の取得したやつ表示
	for (int i = 0;i < player->MyWeaponList_.size();i++) {
		//同じように名前が一緒なら
		for (int j = 0;j < LEVEL::EQUIPMENTSNAMELIST.size();j++) {
			if (player->MyWeaponList_[i]->GetObjectName() == LEVEL::EQUIPMENTSNAMELIST[j]) {
				Transform IconTrans = HUDTransforms_[TRANSFORMTYPE::EQUIPMENTS];
				IconTrans.position_ = { HUDTransforms_[TRANSFORMTYPE::EQUIPMENTS].position_.x + (i * EQUIPMENTS::ICONBUFFERWIDTH),HUDTransforms_[TRANSFORMTYPE::EQUIPMENTS].position_.y,0 };
				Image::SetTransform(hLevelIconImage_[j], IconTrans);
				Image::Draw(hLevelIconImage_[j]);
			}
		}
	}

	//防具の取得したやつ表示
	for (int i = 0;i < player->MyArmorList_.size();i++) {
		//同じように名前が一緒なら
		for (int j = 0;j < LEVEL::EQUIPMENTSNAMELIST.size();j++) {
			if (player->MyArmorList_[i]->GetObjectName() == LEVEL::EQUIPMENTSNAMELIST[j]) {
				Transform IconTrans = HUDTransforms_[TRANSFORMTYPE::EQUIPMENTS];
				IconTrans.position_ = { HUDTransforms_[TRANSFORMTYPE::EQUIPMENTS].position_.x + (i * EQUIPMENTS::ICONBUFFERWIDTH),HUDTransforms_[TRANSFORMTYPE::EQUIPMENTS].position_.y - EQUIPMENTS::ICONBUFFERHEIGHT,0 };
				Image::SetTransform(hLevelIconImage_[j], IconTrans);
				Image::Draw(hLevelIconImage_[j]);
			}
		}
	}

}
