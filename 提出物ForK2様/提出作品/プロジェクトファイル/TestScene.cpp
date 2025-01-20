#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
}

//更新
void TestScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		SceneManager* psManager = (SceneManager*)FindObject("SceneManager");
		psManager->ChangeScene(SCENE_ID_PLAY);
	}
		
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
