#include "GameOver.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
//#include "GamePlayScene.h"
#include "TitleScene.h"

using namespace DirectX;

void GameOver::Initialize()
{
#pragma region 描画初期化処理

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	////---objからモデルデータ読み込み---
	//model_1 = Model::LoadFromOBJ("ground");
	//model_2 = Model::LoadFromOBJ("triangle_mat");
	////Model* model_3 = Model::LoadFromOBJ("chr_sword");
	////---3dオブジェクト生成---
	//object3d_1 = Object3d::Create();
	//object3d_2 = Object3d::Create();
	//object3d_3 = Object3d::Create();
	////---3dオブジェクトに3dモデルを紐づける---
	//object3d_1->SetModel(model_1);
	//object3d_2->SetModel(model_2);
	//object3d_3->SetModel(model_2);

	//object3d_2->SetScale({ 20.0f, 20.0f, 20.0f });
	//object3d_3->SetScale({ 30.0f, 30.0f, 30.0f });

	//object3d_2->SetPosition({ 5,-1,5 });
	//object3d_3->SetPosition({ -5,-1,5 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("D_rhythmaze_119.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("D_rhythmaze_119.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameOver.png");

	// スプライトの生成
	gameover.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // スプライトの座標変更
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);ClearScene
	//}

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター
}

void GameOver::Finalize()
{

	//スプライト解放
	//delete sprite;

	//3dオブジェクト解放
	//delete object3d_1;
	//delete object3d_2;
	//delete object3d_3;
	//delete model_1;
	//delete model_2;
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	//const bool input1 = input->TriggerKey(DIK_1);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押した瞬間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	input->PadVibrationDef();
	if (PadTriggerA|| TriggerEnter)     // 1が押されていたら
	{
		//振動
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	// 座標操作
	//const bool inputUp = input->PushKey(DIK_UP);
	//const bool inputDown = input->PushKey(DIK_DOWN);
	//const bool inputRight = input->PushKey(DIK_RIGHT);
	//const bool inputLeft = input->PushKey(DIK_LEFT);
	
	DrawUI();

	//DebugText::GetInstance()->Print("nihon kougakuin!", 200, 200, 2.0f);

	////3dobj
	//object3d_1->Update();
	//object3d_2->Update();
	//object3d_3->Update();

	//スプライト更新
	gameover->Update();

}

void GameOver::Draw()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	gameover->Draw();

	////3dオブジェ描画前処理
	//Object3d::PreDraw();

	////3dオブジェ描画
	//object3d_1->Draw();
	//object3d_2->Draw();
	//object3d_3->Draw();

	////3dオブジェ描画後処理
	//Object3d::PostDraw();

	// ４．描画コマンドここから

	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//    DrawObject3d(&object3ds[i], dxBase->GetCmdList(), basicDescHeap.Get(), vbView, ibView,
	//        CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeap->GetGPUDescriptorHandleForHeapStart(), constantBufferNum, dxBase->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)),
	//        indices, _countof(indices));
	//}

	//// スプライト共通コマンド
	//SpriteBase::GetInstance()->PreDraw();
	//SpriteBase(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	//sprite->Draw();
}

void GameOver::DrawUI()
{
	DebugText::GetInstance()->Print("[ENTERorPAD_A]:Title", 430, 100, 3);
}
