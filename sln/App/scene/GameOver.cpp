#include "GameOver.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
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

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("D_rhythmaze_119.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("D_rhythmaze_119.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

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

	sp_retry->SetPosition({ WindowWidthHalf_spPos - 300,WindowHeightHalf_spPos / 2 + 300,0 });
	sp_gotitle->SetPosition({ WindowWidthHalf_spPos + 300,WindowHeightHalf_spPos / 2 + 300,0 });
	sp_Now->SetPosition({ sp_retry->GetPosition().x - 50,sp_retry->GetPosition().y - 50,0 });

#pragma endregion 描画初期化処理
}

void GameOver::Finalize()
{
}

void GameOver::Retry()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	XMFLOAT3 SSOp0_1pos = sp_Now->GetPosition();
	if (waitKeyEase < 20) { waitKeyEase++; }//毎フレ足す

	if (waitKeyEase >= 2)//まってから入力受付
	{
		if (cInput->tRightArrow() || cInput->tRightMove()) {//1を次は選択
			cursorMoveNowFlag = true;//動いてるから入力ダメよ
			selectPattern_ = SelectPattern::goTitle;//[リトライからタイトルへ]に変更
		}
	}

	if ((cInput->Decision()) && cursorMoveNowFlag == false)
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
		if ((SSOp0_1pos.x + (easeVal - decEaseVal)) > (sp_gotitle->GetPosition().x - 50))
		{
			cursorMoveNowFlag = false;//自由にしてよい
			easeVal = easeValDef;//イーズ値初期化
			waitKeyEase = 0;//受付待ち元に戻す
			nowSelect = 1;
			sSOp0_1Flag = false;
		}
	}

	{//移動処理
		//0から1へ移動
		if (sSOp0_1Flag)
		{
			SSOp0_1pos.x += easeVal;
			easeVal -= decEaseVal;
			sp_Now->SetPosition({ SSOp0_1pos.x,sp_gotitle->GetPosition().y - 50,0 });
		}
	}

	//--------↓セレクトパターンenum
	//操作説明からスタートへ
	if (selectPattern_ == SelectPattern::goTitle) {
		selectPattern_ = SelectPattern::def;
		sSOp0_1Flag = true;
	}
	//--------↑セレクトパターンenum
}
void GameOver::GoTitle()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	XMFLOAT3 SSOp1_0pos = sp_Now->GetPosition();
	if (waitKeyEase < 20) { waitKeyEase++; }//毎フレ足す

	if (waitKeyEase >= 2)//まってから入力受付
	{
		if (cInput->tLeftArrow() || cInput->tLeftMove()) {//1を次は選択
			cursorMoveNowFlag = true;//動いてるから入力ダメよ
			selectPattern_ = SelectPattern::retry;//[タイトルへからリトライ]に変更
		}
	}

	if ((cInput->Decision()) && cursorMoveNowFlag == false)
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
		if ((SSOp1_0pos.x - (easeVal - decEaseVal)) < (sp_retry->GetPosition().x - 50))
		{
			cursorMoveNowFlag = false;//自由にしてよい
			easeVal = easeValDef;//イーズ値初期化
			waitKeyEase = 0;//受付待ち元に戻す
			nowSelect = 0;
			sSOp0_1Flag = false;
		}
	}

	{//移動処理
		//0から1へ移動
		if (sSOp0_1Flag)
		{
			SSOp1_0pos.x -= easeVal;
			easeVal -= decEaseVal;
			sp_Now->SetPosition({ SSOp1_0pos.x,sp_gotitle->GetPosition().y - 50,0 });
		}
	}

	//--------↓セレクトパターンenum
	//操作説明からスタートへ
	if (selectPattern_ == SelectPattern::retry) {
		selectPattern_ = SelectPattern::def;
		sSOp0_1Flag = true;
	}
	//--------↑セレクトパターンenum
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	input->PadVibrationDef();

	//メンバ関数ポインタ対応した選択
	if (nowSelect == 0) { pFunc = &GameOver::Retry; }
	if (nowSelect == 1) { pFunc = &GameOver::GoTitle; }

	//メンバ関数ポインタ呼び出し
	(this->*pFunc)();

	DrawUI();

	//スプライト更新
	gameover->Update();
	sp_retry->Update();
	sp_gotitle->Update();
	sp_Now->Update();
}

void GameOver::Draw()
{
}

void GameOver::DrawUI()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	// スプライト描画
	gameover->Draw();
	sp_retry->Draw();
	sp_gotitle->Draw();
	sp_Now->Draw();
}