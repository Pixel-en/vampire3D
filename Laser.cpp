#include "Laser.h"
#include "Player.h"

namespace {
	const float LASERSTARTSIZE{ 0.05f };
	const float LASERSIZERATE{ 0.2f };
	const float LASERANGLERATE{ 2.0f };
	const float LASERWAITTIME{ 0.5f };
}

void Laser::AddBullet()
{
	Laser::cLaser* c = Instantiate<Laser::cLaser>(GetParent());
	c->SetStatus(status_);
	c->SetCollider();
	//止まっているなら残りの時間+バッファ
	if (!List_[0]->isMove()) {
		c->SetResetTimer(List_[0]->GetResetTimer() + (List_.size() * BUFFER));
	}
	//動いているなら残りの攻撃持続時間+リセットタイム+バッファ
	else {
		c->SetResetTimer(List_[0]->GetResetTimer() + List_[0]->GetAttackTimer() + (List_.size() * BUFFER));
	}
	List_.push_back(c);
	LaserDir();
}

void Laser::LaserDir()
{
	int rot = 360 / List_.size();
	for (int i = 0; i < List_.size(); i++) {
		dynamic_cast<cLaser*>(List_[i])->SetAngle(rot * i);
	}
}

Laser::Laser(GameObject* parent)
	:WeaponObject(parent, "Laser")
{
	Laser::cLaser* c = Instantiate<Laser::cLaser>(GetParent());
	c->SetStatus(status_);
	c->SetCollider();
	List_.push_back(c);
}

Laser::~Laser()
{
}

void Laser::Initialize()
{
}

void Laser::Update()
{
	for (int i = 0; i < List_.size(); i++) {
		if (List_[i]->GetLv() != status_.Lv_)
			List_[i]->SetNextStatus(status_);
	}
}

void Laser::Draw()
{
}

void Laser::Release()
{
}


void Laser::cLaser::Move()
{
	//演出のためレーザーを回す
	transform_.rotate_.z -= LASERANGLERATE;

	float sizeboost = 1.0f;
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player != nullptr) {
		sizeboost = player->GetStatus().area_;
	}


	//大きくなるまでのタイマー
	if (BiggerWaittimer_ < 0.0) {
		Clash();

		//徐々に大きく
		if (transform_.scale_.x < status_.size_ * sizeboost)
			transform_.scale_ += {LASERSIZERATE, LASERSIZERATE, 0};
		else {
			transform_.scale_ = { status_.size_ * sizeboost,status_.size_ * sizeboost,transform_.scale_.z * sizeboost };

			//攻撃持続タイマー
			if (varia_.AttackTime_ < 0.0f) {
				Stop();
				return;
			}
			else {
				varia_.AttackTime_ -= Time::DeltaTime();
			}
		}
	}
	else {
		BiggerWaittimer_ -= Time::DeltaTime();
		NonClash();
	}
}

void Laser::cLaser::ResetSub()
{
	XMFLOAT3 StartPos = transform_.position_;

	//前方向ベクトル
	XMVECTOR Front = { 0,0,1,0 };
	//プレイヤーの角度分回転
	XMMATRIX rot = XMMatrixRotationY(transform_.rotate_.y / 180.0f * XM_PI);
	//回転と正規化
	XMVECTOR dir = XMVector3Normalize(XMVector3TransformCoord(Front, rot));
	//初期値をプレイヤーの少し前に
	StartPos += dir * 2;
	//発射原点からレーザー全長の半分にポジションを設定
	transform_.position_ = StartPos + dir * (status_.Range_ / 2.0f);
	//初期の大きさに変更
	transform_.scale_ = { LASERSTARTSIZE,LASERSTARTSIZE ,transform_.scale_.z * status_.Range_ };

	int count = 0;

	float sizeboost = 1.0f;
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player != nullptr) {
		sizeboost = player->GetStatus().area_;
	}

	//コライダーの位置を変更
	for (auto itr = colliderList_.begin(); itr != colliderList_.end(); itr++) {
		//ローカルポジションで出るので先に加算される分を引いてワールドポジションにしておく
		//発射原点+(方向*コライダーの大きさ(コライダーの端と原点を合わせる))+(方向*コライダーの大きさの2つ分*何個目か(間隔をあける))
		(*itr)->SetPosition(Transform::Float3Sub(StartPos + (dir * status_.size_ * sizeboost / 2.0f) + (dir * status_.size_ * sizeboost * count), GetWorldPosition()));
		count++;
	}
}

void Laser::cLaser::CollisionSizeSet()
{
	SetCollider();
}

Laser::cLaser::cLaser(GameObject* parent)
	:WeaponObject(parent, "cLaser")
{
}

Laser::cLaser::~cLaser()
{
}

void Laser::cLaser::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Weapon\\Laser.fbx");
	HandleCheck(hModel_);

	targetName_ = "Enemy";
}

void Laser::cLaser::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Laser::cLaser::Release()
{
}

void Laser::cLaser::SetCollider()
{
	float sizeboost = 1.0f;
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player != nullptr) {
		sizeboost = player->GetStatus().area_;
	}


	ClearCollider();
	//コライダーセット
	for (int i = 0; i < status_.Range_ / (status_.size_ * sizeboost); i++) {
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), (status_.size_ * sizeboost) / 2.0f);
		AddCollider(collision);
	}
}
