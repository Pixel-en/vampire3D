#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

#include "Player.h"
#include "EnemySpawn.h"
#include "Enemy.h"

namespace RADAR{
	const float RADARSCALE{ 1000.0f };			//レーダーのサイズに縮小
	const float RADARRANGE{ 14.0f / 100.0f };	//レーダーの感知範囲
	const XMFLOAT3 RADARPOS{ 0.84f,-0.72f,0.0f };
	const int RADARALPHA{ 200 };
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
}

void HUD::Update()
{
	RadarUpdate();
}

void HUD::Draw()
{
	RadarDraw();
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

	EnemySpawn* ep = GetParent()->FindGameObject<EnemySpawn>();
	assert(ep != nullptr);
	std::vector<Enemy*> EnemyList = ep->GetEnemyList();

	if (EnemyList.empty())
		return;

	REnemyPosList_.clear();

	for (auto I : EnemyList) {

		//プレイヤーと敵の距離が一定以上外なら表示しない
		float EPDistance = I->GetPosition() - player->GetPosition();
		Debug::Log(EPDistance, true);
		if (EPDistance <= -RADAR::RADARRANGE * RADAR::RADARSCALE || EPDistance >= RADAR::RADARRANGE * RADAR::RADARSCALE)
			continue;

		//画像は2次元でxy座標なのでyとz入れ替え
		XMFLOAT3 EPos = { I->GetPosition().x - player->GetPosition().x,I->GetPosition().z - player->GetPosition().z, I->GetPosition().y - player->GetPosition().y };
		EPos.x = EPos.x / RADAR::RADARSCALE;
		//画面の比率によって変わるため比率を合わせる
		EPos.y = EPos.y / RADAR::RADARSCALE * (screenWidth / screenHeight);

		REnemyPosList_.push_back(EPos);
	}

	//ここもzとy入れ替え+逆回転になってるので
	RPlayerTransform_.rotate_.z = -player->GetRotate().y;
}


void HUD::RadarDraw()
{
	Image::SetAlpha(hRadar_, RADAR::RADARALPHA);
	Image::SetAlpha(hREnemy_, RADAR::RADARALPHA);
	Image::SetAlpha(hRPlayer_, RADAR::RADARALPHA);
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