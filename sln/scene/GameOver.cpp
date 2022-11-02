#include "GameOver.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
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
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("D_rhythmaze_119.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameOver.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/GameOver_Retry.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/GameOver_GoTitle.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/StageSelect_Now.png");

	// スプライトの生成
	gameover.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	sp_retry.reset(Sprite::Create(2, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	sp_gotitle.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	sp_Now.reset(Sprite::Create(4, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));

	//スプライトポジション
	//window縦横取得したいとき使う
	WinApp* winApp = WinApp::GetInstance();
	float WindowWidthHalf_spPos = (winApp->window_width / 2) - 100;
	float WindowHeightHalf_spPos = (winApp->window_height / 2) - 100;//-100はスプライト座標修正用　画像サイズ200半分の100

	sp_retry->SetPosition({ WindowWidthHalf_spPos - 300,WindowHeightHalf_spPos / 2+300,0 });
	sp_gotitle->SetPosition({ WindowWidthHalf_spPos + 300,WindowHeightHalf_spPos / 2+300,0 });
	sp_Now->SetPosition({ sp_retry->GetPosition().x - 50,sp_retry->GetPosition().y - 50,0 });
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
}

void GameOver::Finalize()
{

}

void GameOver::Retry()
{
	Input* input = Input::GetInstance();
	//押した瞬間
	const bool TriggerRight = input->TriggerKey(DIK_RIGHT);
	const bool TriggerLeft = input->TriggerKey(DIK_LEFT);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押している間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerLeft = input->TriggerButton(static_cast<int>(Button::LEFT));
	const bool PadTriggerRight = input->TriggerButton(static_cast<int>(Button::RIGHT));

	XMFLOAT3 SSOp0_1pos = sp_Now->GetPosition();
	if (WaitKeyEase < 20) { WaitKeyEase++; }//毎フレ足す

	if (WaitKeyEase >= 2)//まってから入力受付
	{
		if (TriggerRight || PadTriggerRight) {//1を次は選択
			selectPattern_ = SelectPattern::goTitle;//[リトライからタイトルへ]に変更
		}
	}

	if ((TriggerEnter || PadTriggerA) && CursorMoveNowFlag == false)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}

	{//次関数へ
	//イーズ値入れて目標座標超える予定なら
		if ((SSOp0_1pos.x + (EaseVal - DecEaseVal)) > (sp_gotitle->GetPosition().x - 50))
		{
			CursorMoveNowFlag = false;//自由にしてよい
			EaseVal = EaseValDef;//イーズ値初期化
			WaitKeyEase = 0;//受付待ち元に戻す
			NowSelect = 1;
			SSOp0_1Flag = false;
		}
	}

	{//移動処理
		//0から1へ移動
		if (SSOp0_1Flag == true)
		{
			CursorMoveNowFlag = true;//動いてるから入力ダメよ
			SSOp0_1pos.x += EaseVal;
			EaseVal -= DecEaseVal;
			sp_Now->SetPosition({ SSOp0_1pos.x,sp_gotitle->GetPosition().y - 50,0 });
		}
	}

	//--------↓セレクトパターンenum
	//操作説明からスタートへ
	if (selectPattern_ == SelectPattern::goTitle) {

		selectPattern_ = SelectPattern::def;
		SSOp0_1Flag = true;

	}
	//--------↑セレクトパターンenum
}
void GameOver::GoTitle()
{
	Input* input = Input::GetInstance();
	//押した瞬間
	const bool TriggerRight = input->TriggerKey(DIK_RIGHT);
	const bool TriggerLeft = input->TriggerKey(DIK_LEFT);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押している間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerLeft = input->TriggerButton(static_cast<int>(Button::LEFT));
	const bool PadTriggerRight = input->TriggerButton(static_cast<int>(Button::RIGHT));

	XMFLOAT3 SSOp1_0pos = sp_Now->GetPosition();
	if (WaitKeyEase < 20) { WaitKeyEase++; }//毎フレ足す

	if (WaitKeyEase >= 2)//まってから入力受付
	{
		if (TriggerLeft || PadTriggerLeft) {//1を次は選択
			selectPattern_ = SelectPattern::retry;//[タイトルへからリトライ]に変更
		}
	}

	if ((TriggerEnter || PadTriggerA) && CursorMoveNowFlag == false)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	{//次関数へ
	//イーズ値入れて目標座標超える予定なら
		if ((SSOp1_0pos.x - (EaseVal - DecEaseVal)) < (sp_retry->GetPosition().x - 50))
		{
			CursorMoveNowFlag = false;//自由にしてよい
			EaseVal = EaseValDef;//イーズ値初期化
			WaitKeyEase = 0;//受付待ち元に戻す
			NowSelect = 0;
			SSOp0_1Flag = false;
		}
	}

	{//移動処理
		//0から1へ移動
		if (SSOp0_1Flag == true)
		{
			CursorMoveNowFlag = true;//動いてるから入力ダメよ
			SSOp1_0pos.x -= EaseVal;
			EaseVal -= DecEaseVal;
			sp_Now->SetPosition({ SSOp1_0pos.x,sp_gotitle->GetPosition().y - 50,0 });
		}
	}

	//--------↓セレクトパターンenum
	//操作説明からスタートへ
	if (selectPattern_ == SelectPattern::retry) {

		selectPattern_ = SelectPattern::def;
		SSOp0_1Flag = true;

	}
	//--------↑セレクトパターンenum
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	//const bool input1 = input->TriggerKey(DIK_1);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押した瞬間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	input->PadVibrationDef();

	//メンバ関数ポインタ対応した選択
	if (NowSelect == 0) { pFunc = &GameOver::Retry; }
	if (NowSelect == 1) { pFunc = &GameOver::GoTitle; }

	//メンバ関数ポインタ呼び出し
	(this->*pFunc)();

	//if (PadTriggerA|| TriggerEnter)     // 1が押されていたら
	//{

	//	GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
	//	//振動
	//	input->PadVibration();
	//	// 音声停止
	//	GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
	//	//シーン切り替え
	//	BaseScene* scene = new TitleScene();
	//	sceneManager_->SetNextScene(scene);
	//}

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
	sp_retry->Update();
	sp_gotitle->Update();
	sp_Now->Update();

}

void GameOver::Draw()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	gameover->Draw();
	sp_retry->Draw();
	sp_gotitle->Draw();
	sp_Now->Draw();
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
	//DebugText::GetInstance()->Print("[ENTERorPAD_A]:Title", 430, 100, 3);
}
