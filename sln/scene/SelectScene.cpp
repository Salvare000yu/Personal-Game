#include "SelectScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"
#include "WinApp.h"
#include "TitleScene.h"

#include "PostEffect.h"

using namespace DirectX;

void SelectScene::Initialize()
{
	//window縦横取得したいとき使う
	WinApp* winApp = WinApp::GetInstance();

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("C_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

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

	sp_SSNow->SetPosition({ sp_SSoper->GetPosition().x - 50,sp_SSoper->GetPosition().y - 50,0 });
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

	XMFLOAT3 SSOp0_1pos = sp_SSNow->GetPosition();

	if (WaitKeyEase < 20) { WaitKeyEase++; }//毎フレ足す
	//操作説明開いてないときのみ
	if (OperWindOpenFlag == false && WaitKeyEase >= 2)//まってから入力受付
	{
		if (TriggerRight || PadTriggerRight) {//1を次は選択
			selectPattern_ = SelectPattern::operationTOstart;//[操作説明からスタートへ]に変更
		}
		//if (TriggerLeft || PadTriggerLeft) {
		//	NowSelect = 2;
		//}
	}

	//操作説明画面
	if (SSOp0_1Flag == false)
	{
		if (TriggerEnter || PadTriggerA && CursorMoveNowFlag == false)
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			OperWindOpenFlag = true;
			OperationWind();

			WaitKeyEnter++;
			if ((TriggerEnter || PadTriggerA) && WaitKeyEnter >= 2 && CursorMoveNowFlag == false) {
				OperWindOpenFlag = false;
				WaitKeyEnter = 0;
			}
		}
	}

	{//次関数へ
		//イーズ値入れて目標座標超える予定なら
		if((SSOp0_1pos.x + (EaseVal - DecEaseVal))> (sp_SSstart->GetPosition().x - 50))
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
			sp_SSNow->SetPosition({ SSOp0_1pos.x,sp_SSoper->GetPosition().y - 50,0 });
		}
	}

	//--------↓セレクトパターンenum
	//操作説明からスタートへ
	if (selectPattern_ == SelectPattern::operationTOstart) {

		selectPattern_ = SelectPattern::def;
		SSOp0_1Flag = true;

	}
	//--------↑セレクトパターンenum
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

	XMFLOAT3 SSStartPos = sp_SSNow->GetPosition();

	if (WaitKeyEase < 20) { WaitKeyEase++; }//毎フレ足す
	if (WaitKeyEase >= 2 && CursorMoveNowFlag == false)
	{
		if (TriggerRight || PadTriggerRight) {
			if (WaitKeyEase >= 2)//入力待ち
			{
				selectPattern_ = SelectPattern::startTOtitle;//[スタートからタイトル]に変更
			}
		}
		if (TriggerLeft || PadTriggerLeft) {
			if (WaitKeyEase >= 2)//入力待ち
			{
				selectPattern_ = SelectPattern::startTOoperation;//[スタートから操作説明へ]に変更
			}
		}
	}

	//プレイシーンへ
	if (SSStart1_2Flag == false&& SSStart1_0Flag==false)
	{
		if (TriggerEnter || PadTriggerA && CursorMoveNowFlag == false)
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();
			// 音声停止
			GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
			//シーン切り替え
			BaseScene* scene = new GamePlayScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	//----------------座標到達で次関数↓
	{//次関数へ
		//イーズ値入れて目標座標超える予定なら
		if ((SSStartPos.x + (EaseVal - DecEaseVal)) > (sp_SStitle->GetPosition().x - 50))
		{
			EaseVal = EaseValDef;//イーズ値初期化
			CursorMoveNowFlag = false;//自由にしてよい
			WaitKeyEase = 0;//受付待ち元に戻す
			NowSelect = 2;
			SSStart1_2Flag = false;
		}
	}
	{//次関数へ
		//イーズ値入れて目標座標超える予定なら
		if ((SSStartPos.x - (EaseVal - DecEaseVal)) < (sp_SSoper->GetPosition().x - 50))
		{
			EaseVal = EaseValDef;//イーズ値初期化
			CursorMoveNowFlag = false;//自由にしてよい
			WaitKeyEase = 0;//受付待ち元に戻す
			NowSelect = 0;
			SSStart1_0Flag = false;
		}
	}
	//----------------座標到達で次関数↑

	//----------------移動処理↓
	{//移動処理
		//1から2へ移動
		if (SSStart1_2Flag == true)
		{
			CursorMoveNowFlag = true;//動いてるから入力ダメよ
			SSStartPos.x += EaseVal;
			EaseVal -= DecEaseVal;
			sp_SSNow->SetPosition({ SSStartPos.x,sp_SSstart->GetPosition().y - 50,0 });
		}
	}
	{//移動処理
	//1から0へ移動
		if (SSStart1_0Flag == true)
		{
			CursorMoveNowFlag = true;//動いてるから入力ダメよ
			SSStartPos.x -= EaseVal;
			EaseVal -= DecEaseVal;
			sp_SSNow->SetPosition({ SSStartPos.x,sp_SSstart->GetPosition().y - 50,0 });
		}
	}
	//----------------移動処理↑

	//--------↓セレクトパターンenum
	//スタートからタイトルへ
	if (selectPattern_ == SelectPattern::startTOtitle) {
		SSStart1_2Flag = true;
		selectPattern_ = SelectPattern::def;

	}
	//スタートから操作説明へ
	if (selectPattern_ == SelectPattern::startTOoperation) {
		SSStart1_0Flag = true;
		selectPattern_ = SelectPattern::def;

	}
	//--------↑セレクトパターンenum
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

	XMFLOAT3 SStitle2_1 = sp_SSNow->GetPosition();

	if (WaitKeyEase < 20) { WaitKeyEase++; }//毎フレ足す
	if (WaitKeyEase >= 2 && CursorMoveNowFlag == false)
	{
		//if (TriggerRight || PadTriggerRight) {
		//	NowSelect = 0;
		//}
		if (TriggerLeft || PadTriggerLeft) {
			selectPattern_ = SelectPattern::titleTOstart;//[タイトルからスタートへ]に変更
		}
	}

	//タイトルシーンへ
	if (SStitle2_1Flag==false)
	{
		if (TriggerEnter || PadTriggerA && CursorMoveNowFlag == false)
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();
			// 音声停止
			GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
			//シーン切り替え
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	{//次関数へ
		//イーズ値入れて目標座標超える予定なら
		if ((SStitle2_1.x - (EaseVal - DecEaseVal)) < (sp_SSstart->GetPosition().x - 50))
		{
			EaseVal = EaseValDef;//イーズ値初期化
			CursorMoveNowFlag = false;//自由にしてよい
			WaitKeyEase = 0;//受付待ち元に戻す
			NowSelect = 1;
			SStitle2_1Flag = false;
		}
	}

	{//移動処理
		//2から1へ移動
		if (SStitle2_1Flag == true)
		{
			CursorMoveNowFlag = true;//動いてるから入力ダメよ
			SStitle2_1.x -= EaseVal;
			EaseVal -= DecEaseVal;
			sp_SSNow->SetPosition({ SStitle2_1.x,sp_SStitle->GetPosition().y - 50,0 });
		}
	}

	//--------↓セレクトパターンenum
	//操作説明からスタートへ
	if (selectPattern_ == SelectPattern::titleTOstart) {
		SStitle2_1Flag = true;
		selectPattern_ = SelectPattern::def;

	}
	//--------↑セレクトパターンenum
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
	////イージング中入力待機時間的な
	//char tmp[32]{};
	//sprintf_s(tmp, 32, "%2.f", WaitKeyEase);
	//DebugText::GetInstance()->Print(tmp, 430, 430, 3);
}
