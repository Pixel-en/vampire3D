#include "PoisonThrow.h"
#include "Player.h"

namespace {
	const int THROWANGLE{ -45 };
	const float ATTACKTIME{ 5.0f };
}

void PoisonThrow::Move()
{
	XMVECTOR frontVec{ 0,0,1,0 };

	//角度分ベクトルを上に向けるマトリクスを作る
	XMMATRIX rotup = XMMatrixRotationX(XMConvertToRadians(THROWANGLE));

	//ベクトルを上に向けて、正規化
	XMVECTOR dirVec = XMVector3TransformCoord(frontVec, rotup);
	dirVec = XMVector3Normalize(dirVec);

	XMMATRIX rotM = XMMatrixRotationY(XMConvertToRadians(angle_));

	dirVec = XMVector3Transform(dirVec, rotM);
	dirVec = XMVector3Normalize(dirVec);

	transform_.position_ += dirVec * 30.0f * Time::DeltaTime();

}

void PoisonThrow::Reset()
{
	Player* player = GetRootJob()->FindGameObject<Player>();

	transform_ = player->GetTransform();
	originPos = player->GetPosition();
	attackTimer_ = ATTACKTIME;

	Visible();
	Clash();
	allowsMove_ = true;


	angle_ = rand() % 360;
}

PoisonThrow::PoisonThrow(GameObject* parent)
	:WeaponObject(parent,"PoisonThrow")
{
}

PoisonThrow::~PoisonThrow()
{
}

void PoisonThrow::Initialize()
{
	hModel_ = Model::Load("Assets\\Model\\Box.fbx");
	assert(hModel_ >= 0);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	AddCollider(collision);
}


void PoisonThrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void PoisonThrow::Release()
{
}

void PoisonThrow::OnCollision(GameObject* pTarget)
{
}
