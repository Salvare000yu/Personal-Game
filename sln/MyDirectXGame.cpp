#include "MyDirectXGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"
#include "EndScene.h"

void MyDirectXGame::Initialize()
{
    //基底クラスの初期化処理
    GameBase::Initialize();

#pragma region シーン初期化処理
    BaseScene* scene = new TitleScene();
    //シーンマネージャに最初のシーンセット
    sceneManager_->SetNextScene(scene);

#pragma region シーン初期化処理

}

void MyDirectXGame::Finalize()
{
 
    //基底クラスの終了処理
    GameBase::Finalize();

}

void MyDirectXGame::Update()
{
    //基底クラスの更新処理
    GameBase::Update();

    // DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理
}

void MyDirectXGame::Draw()
{
    GameBase::Draw();
}
