#include "PlayLoad.h"
#include "Engine/Image.h"

PlayLoad::PlayLoad(GameObject* parent)
	:GameObject(parent, "PlayLoad"), hBGImage_(-1), hLDImage_(-1)
{
}

PlayLoad::~PlayLoad()
{
}

void PlayLoad::Initialize()
{
	hBGImage_ = Image::Load("Assets\\Image\\UI\\BlackBack.png");
	HandleCheck(hBGImage_, "ロード画面の背景画像がない");
	hLDImage_ = Image::Load("Assets\\Image\\UI\\LoadCircle.png");
	HandleCheck(hLDImage_, "ロード中の画像がない");


	trans.position_ = { 0.8,-0.7,0 };
}

void PlayLoad::Update()
{
	trans.rotate_.z -= 1;
}

void PlayLoad::Draw()
{
	Image::SetTransform(hBGImage_, transform_);
	Image::Draw(hBGImage_);
	Image::SetTransform(hLDImage_, trans);
	Image::Draw(hLDImage_);
}

void PlayLoad::Release()
{
}
