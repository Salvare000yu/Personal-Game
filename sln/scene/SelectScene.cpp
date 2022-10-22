#include "SelectScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"
#include "WinApp.h"
#include "TitleScene.h"

#include "../PostEffect.h"

using namespace DirectX;

void SelectScene::Initialize()
{
	//window縦横取得したいとき使う
	WinApp* winApp = WinApp::GetInstance();

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("C_rhythmaze_125.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("C_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/StageSelect.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/StageSelect_oper.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/StageSelect_Start.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/StageSelect_Title.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/StageSelect_Now.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/operation_wind.png");

	// スプライトの生成
	sp_stageselect.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SSoper.reset(Sprite::Create(2, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SSstart.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SStitle.reset(Sprite::Create(4, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SSNow.reset(Sprite::Create(5, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation_wind.reset(Sprite::Create(6, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//スプライトポジション
	float WindowWidthHalf_spPos = (winApp->window_width / 2) - 100;
	float WindowHeightHalf_spPos = (winApp->window_height / 2) - 100;//-100はスプライト座標修正用　画像サイズ200半分の100

	sp_SSoper->SetPosition({ WindowWidthHalf_spPos - 400,WindowHeightHalf_spPos,0 });
	sp_SSstart->SetPosition({ WindowWidthHalf_spPos,WindowHeightHalf_spPos,0 });
	sp_SStitle->SetPosition({ WindowWidthHalf_spPos + 400,WindowHeightHalf_spPos,0 });
}

void SelectScene::SelectOper()
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

	//選択中画像表示 -50は枠の縦横分
	sp_SSNow->SetPosition({ sp_SSoper->GetPosition().x - 50,sp_SSoper->GetPosition().y - 50,0 });

	if (TriggerRight || PadTriggerRight) {//1を次は選択
		NowSelect = 1;
	}
	if (TriggerLeft || PadTriggerLeft) {
		NowSelect = 2;
	}

	//操作説明画面
	if (TriggerEnter || PadTriggerA)
	{
		OperWindOpenFlag = true;
		OperationWind();

		WaitKeyEnter++;
		if ((TriggerEnter || PadTriggerA) && WaitKeyEnter >= 2) {
			OperWindOpenFlag = false;
			WaitKeyEnter = 0;
		}

	}

}
void SelectScene::OperationWind()
{
	sp_operation_wind->Update();
}

void SelectScene::SelectStart()
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

	//選択中画像表示 -50は枠の縦横分
	sp_SSNow->SetPosition({ sp_SSstart->GetPosition().x - 50,sp_SSstart->GetPosition().y - 50,0 });

	if (TriggerRight || PadTriggerRight) {
		NowSelect = 2;
	}
	if (TriggerLeft || PadTriggerLeft) {
		NowSelect = 0;
	}

	//プレイシーンへ
	if (TriggerEnter || PadTriggerA)
	{
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}
}
void SelectScene::SelectTitle()
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

	//選択中画像表示 -50は枠の縦横分
	sp_SSNow->SetPosition({ sp_SStitle->GetPosition().x - 50,sp_SStitle->GetPosition().y - 50,0 });

	if (TriggerRight || PadTriggerRight) {
		NowSelect = 0;
	}
	if (TriggerLeft || PadTriggerLeft) {
		NowSelect = 1;
	}

	//タイトルシーンへ
	if (TriggerEnter || PadTriggerA)
	{
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}
}

void SelectScene::Finalize()
{
}

void SelectScene::Update()
{
	Input* input = Input::GetInstance();

	//押した瞬間のみ
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//パッド押した瞬間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));

	if (TriggerSPACE || PadTriggerA)     // スペースキーが押されていたら
	{
		//input->PadVibration();
		//// 音声停止
		//GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
		////シーン切り替え
		//BaseScene* scene = new SelectScene();
		//sceneManager_->SetNextScene(scene);
	}

	//タイトルから振動少し続ける
	if (--VibCount == 0) {
		input->PadVibrationDef();
	}

	//メンバ関数ポインタ対応した選択
	if (NowSelect == 0) { pFunc = &SelectScene::SelectOper; }
	if (NowSelect == 1) { pFunc = &SelectScene::SelectStart; }
	if (NowSelect == 2) { pFunc = &SelectScene::SelectTitle; }

	//メンバ関数ポインタ呼び出し
	(this->*pFunc)();

	DrawUI();

	//スプライト更新
	sp_stageselect->Update();
	sp_SSoper->Update();
	sp_SSstart->Update();
	sp_SStitle->Update();
	sp_SSNow->Update();
}

void SelectScene::Draw()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(SspriteBase, dxBase->GetCmdList());
	//// スプライト描画
	sp_stageselect->Draw();
	sp_SSoper->Draw();
	sp_SSstart->Draw();
	sp_SStitle->Draw();
	sp_SSNow->Draw();
	if (OperWindOpenFlag == true) { sp_operation_wind->Draw(); }
}

void SelectScene::DrawUI()
{
}
