#include "Enemy.h"
#include "Field.h"
#include "Player.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include <algorithm>


using std::string;
namespace {
	const float GRAVITY{ 9.8f / 2.0f / 60.0f };
	const float HIGHDISTANCE{ 50.0f };
	const float MIDDLEDISTANCE{ 70.0f };
	const float LOWDISTANCE{ 120.0f };
	
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"")
{
}

Enemy::Enemy(GameObject* parent, const std::string& name)
	:GameObject(parent,name)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	ModelHP_ = HP::FULL;
	ModelLOD_ = LOD::HIGH;
	ModelAnim_ = ANIMATION::MOVE;
}

void Enemy::Load(LEVEL _level,unsigned int _number)
{
	status_.number_ = _number;
	status_.level_ = _level;

	string Level[LEVEL::END] = { "Blue","Yellow","Green","Red" };

	//CSVからステータスの読み込み
	CsvReader csv;
	csv.Load("Assets\\CSV\\EnemyStatus.csv");
	for (int i = 1;i < csv.GetHeight();i++) {
		if (csv.GetString(0, i) == objectName_ && csv.GetString(1,i)==Level[_level]) {
			status_.power_ = csv.GetValue(2, i);
			status_.speed_ = csv.GetValue(3, i);
			status_.hp_ = csv.GetValue(4, i);
			status_.exp_ = csv.GetValue(5, i);
		}
	}

	//モデルのロード
	string hp[HP::MAX] = { "Full", "Half", "Mimi" };
	string anim[ANIMATION::MAX] = { "Move", "Hit", "Death" };
	string lod[LOD::MAX] = { "", "Middle", "Low" };
	for (int i = 0;i < HP::MAX;i++) {
		for (int j = 0;j < LOD::MAX;j++) {
			for (int k = 0;k < ANIMATION::MAX;i + k++) {
				hModel_[i][j][k] = Model::Load("Assets\\Model\\Character\\Enemy\\Enemy-"+Level[status_.level_] +"-" + hp[i]+ "-" + lod[j] + "-" + anim[k] + ".fbx");
				HandleCheck(hModel_[i][j][k], Level[status_.level_] + "," + hp[i] + "," + lod[j] + "," + anim[k]+ "のEnemyモデルがない");

			}
		}
	}

}

void Enemy::Update()
{
}

void Enemy::Move()
{
	Field* field = GetRootJob()->FindGameObject<Field>();
	NullCheck(field);
	Player* player = GetRootJob()->FindGameObject<Player>();
	NullCheck(player);

	//重力方向
	XMVECTOR Gravity = XMVECTOR{ 0,0,0 };

	//重力方向のベクトルを作る
	if (!onGround_) {
		gravity_ += GRAVITY;
		Gravity = XMVECTOR{ 0,-1,0 };
	}
	else {
		Gravity = XMVECTOR{ 0,0,0 };
		gravity_ = 0.0f;
	}

	XMFLOAT3 pPos = player->GetPosition();
	XMFLOAT3 ePos = transform_.position_;

	XMVECTOR pVec = XMLoadFloat3(&pPos);
	XMVECTOR eVec = XMLoadFloat3(&ePos);

	//プレイヤーと敵のベクトルを取る
	XMVECTOR epDistance = pVec - eVec;

	//LODの設定
	ModelLOD_ = LOD::MAX;
	if (XMVectorGetX(XMVector3Length(epDistance)) <= LOWDISTANCE) {
		ModelLOD_ = LOD::LOW;
		if (XMVectorGetX(XMVector3Length(epDistance)) <= MIDDLEDISTANCE) {
			ModelLOD_ = LOD::MIDDLE;
			if (XMVectorGetX(XMVector3Length(epDistance)) <= HIGHDISTANCE) {
				ModelLOD_ = LOD::HIGH;	
			}
		}
		
	}

	//回転のマトリクスを作る
	XMMATRIX RotMat = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);

	//マトリクスに合わせて前方向のベクトルを回転
	XMVECTOR eFrontVec = XMVector3TransformCoord(XMVectorSet(0, 0, 1, 0), RotMat);

	epDistance = XMVector3Normalize(epDistance);
	eFrontVec = XMVector3Normalize(eFrontVec);

	float dot = XMVectorGetX(XMVector3Dot(eFrontVec, epDistance));	//内積を求める
	//内積がacosの範囲外に出たとき除外する
	dot = std::clamp(dot, -1.0f, 1.0f);

	float angle = acos(dot);	//角度を出す

	//外積を求めてどちらの方向に回転するか決める
	XMVECTOR cross = XMVector3Cross(epDistance, eFrontVec);
	cross = XMVector3Normalize(cross);
	if (XMVectorGetY(cross) >= 0) {
		transform_.rotate_.y += -XMConvertToDegrees(angle);
	}
	else {
		transform_.rotate_.y += XMConvertToDegrees(angle);
	}

	prePos_ = transform_.position_;
	transform_.position_ += epDistance * status_.speed_ * Time::DeltaTime() + Gravity * gravity_;

}

void Enemy::Draw()
{
}

void Enemy::Release()
{
}

void Enemy::HitDamege(int _damege, float _knock)
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	float dBoost = 1.0;
	if (player != nullptr) {
		dBoost = player->GetStatus().strength_;
		if (rand() % 100 < player->GetStatus().critical_) {
			dBoost *= player->GetStatus().criticalBoost_;
		}
	}


}

void Enemy::OnCollisionsList(GameObject* pTarget, std::list<Collider*>::iterator MyItr, std::list<Collider*> list)
{
	if (pTarget->GetObjectName() == "Field") {
		//移動前の戻す
		transform_.position_ = prePos_;

		//プレイヤーの位置を取得
		Player* player = GetRootJob()->FindGameObject<Player>();
		XMVECTOR VecX = XMVector3Normalize(XMVectorSet(player->GetPosition().x - transform_.position_.x, 0, 0, 0));
		XMVECTOR VecZ = XMVector3Normalize(XMVectorSet(0, 0, player->GetPosition().z - transform_.position_.z, 0));

		transform_.position_ += VecX * status_.speed_ * Time::DeltaTime();

		bool hit = false;

		for (auto TargetItr = list.begin(); TargetItr != list.end(); ++TargetItr) {
			if ((*MyItr)->IsHit(*TargetItr)) {
				transform_.position_ = prePos_;
				hit = true;
			}
		}
		if (!hit)
			prePos_ = transform_.position_;

		hit = false;
		transform_.position_ += VecZ * status_.speed_ * Time::DeltaTime();
		for (auto TargetItr = list.begin(); TargetItr != list.end(); ++TargetItr) {
			if ((*MyItr)->IsHit(*TargetItr)) {
				transform_.position_ = prePos_;
				hit = true;
			}
		}
		if (!hit)
			prePos_ = transform_.position_;

	}
}
