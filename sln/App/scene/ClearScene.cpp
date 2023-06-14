#include "ClearScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "TitleScene.h"

using namespace DirectX;

void ClearScene::Initialize()
{
#pragma region 描画初期化処理

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("verROOP_tukawanakutemoiiYO.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("verROOP_tukawanakutemoiiYO.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameClear.png");

	// スプライトの生成
	sprite.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));

#pragma endregion 描画初期化処理
}

void ClearScene::Finalize()
{
}

void ClearScene::Update()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	if ((cInput->Decision()))
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		//振動
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("verROOP_tukawanakutemoiiYO.wav");
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	DrawUI();

	//スプライト更新
	sprite->Update();
}

void ClearScene::Draw()
{
}

void ClearScene::DrawUI()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//// スプライト描画
	sprite->Draw();
}