#include "PoisonThrow.h"
#include "Player.h"
#include "Field.h"
#include "EnemySpawn.h"

namespace {
	const int THROWANGLE{ -30 };
	const float RAYHEIGHT{ 5.0f };
	const float RAYLIMIT{ 1.0f };
	const float GRAVITY{ 0.0001f };
	const float THROWHEIGHT{ 2.0f };	//投げ始めの高さ
}

void PoisonThrow::AddBullet()
{
	PoisonThrow::cPoisonThrow* c = Instantiate<PoisonThrow::cPoisonThrow>(GetParent());
	c->SetStatus(status_);
	//止まっているなら残りの時間+バッファ
	if (!List_[0]->isMove()) {
		c->SetResetTimer(List_[0]->GetResetTimer() + (List_.size() * BUFFER));
	}
	//動いているなら残りの攻撃持続時間+リセットタイム+バッファ
	else {
		c->SetResetTimer(List_[0]->GetResetTimer() + List_[0]->GetAttackTimer() + (List_.size() * BUFFER));
	}
	List_.push_back(c);

}

PoisonThrow::PoisonThrow(GameObject* parent)
	:WeaponObject(parent,"PoisonThrow")
{
	cPoisonThrow* c = Instantiate<cPoisonThrow>(GetParent());
	List_.push_back(c);
}

PoisonThrow::~PoisonThrow()
{
}

void PoisonThrow::Initialize()
{
}

void PoisonThrow::Update()
{
	for (int i = 0; i < List_.size(); i++) {
		if (List_[i]->GetLv() != status_.Lv_)
			List_[i]->SetNextStatus(status_);
	}
}

void PoisonThrow::Draw()
{
}

void PoisonThrow::Release()
{
}

/*------------ここからcPoisonThrow-----------*/

void PoisonThrow::cPoisonThrow::Move()
{
	Field* field = GetParent()->FindGameObject<Field>();
	if (field->RayCastField(transform_.position_, RAYHEIGHT, RAYLIMIT)) {
		//地面着地後のプログラム
		Clash();
		if (varia_.AttackTime_ < 0.0f)
			Stop();
		else {
			varia_.AttackTime_ -= Time::DeltaTime();
		}

		return;
	}

	NonClash();
	move_ += XMVectorSet(0, gravity_, 0, 0);
	gravity_ -= GRAVITY;

	transform_.position_ += move_ * status_.speed_ * Time::DeltaTime();
	if (transform_.position_.y <= -10.0f)
		Stop();
}

void PoisonThrow::cPoisonThrow::ResetSub()
{
	transform_.position_.y += 2.0f;

	gravity_ = 0;
	angle_ = rand() % 360;

	XMVECTOR frontVec{ 0,0,1,0 };

	//角度分ベクトルを上に向けるマトリクスを作る
	XMMATRIX rotup = XMMatrixRotationX(XMConvertToRadians(THROWANGLE));

	//ベクトルを上に向けて、正規化
	XMVECTOR dirVec = XMVector3TransformCoord(frontVec, rotup);
	dirVec = XMVector3Normalize(dirVec);

	XMMATRIX rotM = XMMatrixRotationY(XMConvertToRadians(angle_));

	move_ = XMVector3Transform(dirVec, rotM);
	move_ = XMVector3Normalize(move_);
}

PoisonThrow::cPoisonThrow::cPoisonThrow(GameObject* parent)
	:WeaponObject(parent,"cPoisonThrow")
{
}

PoisonThrow::cPoisonThrow::~cPoisonThrow()
{
}

void PoisonThrow::cPoisonThrow::Initialize()
{

	hModel_ = Model::Load("Assets\\Model\\PoisonArea.fbx");
	assert(hModel_ >= 0);

	hCapsule_ = Model::Load("Assets\\Model\\Capsule_Blue.fbx");
	assert(hCapsule_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), status_.size_);
	AddCollider(collision);
}

void PoisonThrow::cPoisonThrow::Draw()
{
	if (IsClash()) {
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	}
	else {
		Model::SetTransform(hCapsule_, transform_);
		Model::Draw(hCapsule_);
	}
}

void PoisonThrow::cPoisonThrow::Release()
{
}
