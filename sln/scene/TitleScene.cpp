#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"

#include "Timer.h"

#include "PostEffect.h"

//PostEffect* postEffect = nullptr;

using namespace DirectX;

void TitleScene::Initialize()
{
#pragma region 描画初期化処理

	WinApp* winApp = WinApp::GetInstance();

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	////---objからモデルデータ読み込み---
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	mod_ground.reset(Model::LoadFromOBJ("ground"));
	////---3dオブジェクト生成---
	obj_tunnel.reset(Object3d::Create());
	obj_ground.reset(Object3d::Create());
	////---3dオブジェクトに3dモデルを紐づける---
	obj_tunnel->SetModel(mod_tunnel.get());
	obj_ground->SetModel(mod_ground.get());
	//------object3dスケール------//
	obj_tunnel->SetScale({ 100.0f, 40.0f, 40.0f });
	obj_ground->SetScale({ 80.0f, 20.0f, 500.0f });
	//------object3d位置------//
	obj_tunnel->SetPosition({ 0,40,2000 });
	obj_ground->SetPosition({ 0,-150,0 });
	//------object回転------//
	obj_tunnel->SetRotation({ 0,-90,0 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/GameTitleName.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/Title_oper.png");

	// スプライトの生成
	sprite1.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_gametitlename.reset(Sprite::Create(2, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_titleoper.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//スプライトポジション
	sp_gametitlename->SetPosition({ winApp->window_width,NamePosYCenter,0 });

	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // スプライトの座標変更
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);
	//}

	//ポストエフェクト用テクスチャ読み込み
	//SpriteBase::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	//ポストエフェクト初期化
//	postEffect = new PostEffect();
////	postEffect->Initialize();//100, { 0.0f, 0.0f }, false, falseいちおう
//	postEffect->SetSize({ 500.0f,500.0f });
//	postEffect->SetPosition({ 10,10,0 });
//	postEffect->TransferVertexBuffer();
#pragma endregion 描画初期化処理
}

void TitleScene::Finalize()
{
//	delete postEffect;
}

void TitleScene::BeforeUpdate()
{
	//window縦横取得したいとき使う
	WinApp* winApp = WinApp::GetInstance();

	const float spAccel = 1.05f;//加速

	//中心を取る
	XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	XMFLOAT2 NameTexSize = sp_gametitlename->GetTexSize();

	NamePosXCenter = (winApp->window_width / 2) - (NameTexSize.x / 2);//画像左上にセットされるから中央を取る
	NamePosYCenter = (winApp->window_height / 2) - (NameTexSize.y / 2);//〃;

	if (NamePos.x > NamePosXCenter)
	{
		NamePos.x -= sp;
		sp *= spAccel;
	}
	else { 
		MoveStartFlag = false; 
	}

	sp_gametitlename->SetPosition({ NamePos });
	//XMFLOAT2 NameSize = sp_gametitlename->GetSize();
	//NameSize.x--;
	//NameSize.y--;
	//sp_gametitlename->SetSize({NameSize});
	//sp_gametitlename->TransferVertexBuffer();

	//sp_gametitlename->SetPosition({ NamePosXCenter,NamePosYCenter,NamePos.z });
	sp_gametitlename->Update();
}

void TitleScene::SceneChange()
{
	XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	const float StartSpAccel = 1.05f;

	NamePos.x -= StartSp;
	StartSp *= StartSpAccel;
	sp_gametitlename->SetPosition({ NamePos });

	Input* input = Input::GetInstance();

	if (--SceneChangeVibCount == 0) {
		input->PadVibrationDef();
	}
	if (NamePos.x < -1280) {
		// 音声停止
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}

	sp_gametitlename->Update();
}

void TitleScene::UpDown()
{
	XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	//NamePosYUpDown*=0.99;
	//switch (upDownPattern_)
	//{
	//case UpDownPattern::def:
	//	NamePos.y += NamePosYUpDown;
	//	if(NamePos.y>= NamePosMoveMax){ 
	//		NamePosYUpDown = NamePosYUpDownDef;//デフォルト値に戻す
	//		upDownPattern_ = UpDownPattern::Down; 
	//	}
	//	break;
	//case UpDownPattern::Up:
	//	NamePos.y += NamePosYUpDown;
	//	if (NamePos.y >= NamePosMoveMax) { 
	//		NamePosYUpDown = NamePosYUpDownDef;//デフォルト値に戻す
	//		upDownPattern_ = UpDownPattern::Down; 
	//	}
	//	break;
	//case UpDownPattern::Down:
	//	NamePos.y -= NamePosYUpDown;
	//	if(NamePos.y<=NamePosMoveMin){
	//		NamePosYUpDown = NamePosYUpDownDef;//デフォルト値に戻す
	//		upDownPattern_ = UpDownPattern::Up; 
	//	}
	//	break;
	time=frame / 60;
	NamePos.y += sinf(time*6.f);
	frame++;
	sp_gametitlename->SetPosition({ NamePos });
	sp_gametitlename->Update();
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	////------------------------デバッグ用！！！！！！
	const bool InputSPACE = input->PushKey(DIK_SPACE);
	const bool InputENTER = input->PushKey(DIK_RETURN);
	if (InputSPACE && InputENTER) {
		// 音声停止
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}
	////------------------------デバッグ用！！！！！！

	//セレクトから振動少し続ける
	if (--VibCount == 0) {
		input->PadVibrationDef();
	}

	sprite1->Update();
	if (MoveStartFlag == true) { BeforeUpdate(); }

	if (MoveStartFlag == false && SceneChangeFlag == false)
	{
		
		//押した瞬間のみ
		//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
		//const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
		//パッド押した瞬間
		//const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));

		if ((cInput->DecisionByEnter()))     // スペースキーが押されていたら
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			SceneChangeFlag = true;//チェンジ移動フラグ立てる
			input->PadVibration();
		}

		UpDown();

		//postEffect->Update();
	}

	if (SceneChangeFlag == true) {
		SceneChange();//チェンジ移動開始
	}

	obj_tunnel->Update();
	obj_ground->Update();

	DrawUI();
}

void TitleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();
	//3dオブジェ描画前処理
	Object3d::PreDraw(DxBase::GetInstance()->GetCmdList());

	//３DオブジェクトDraw
	obj_tunnel->Draw();
	obj_ground->Draw();

	//3dオブジェ描画後処理
	Object3d::PostDraw();

	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//// スプライト描画
	//sprite1->Draw();

	sp_gametitlename->Draw();

	if (MoveStartFlag == false && SceneChangeFlag == false)
	{
		sp_titleoper->Draw();
	}

}

void TitleScene::DrawUI()
{
	//DebugText::GetInstance()->Print("[ENTERorGAMEPAD:A] PLAYSCENE", 300, 100, 3.0f);
	//if (MoveStartFlag == true) { DebugText::GetInstance()->Print("MoveStartF:true", 300, 100, 3.0f); }
	//else { DebugText::GetInstance()->Print("MoveStartF:false", 300, 100, 3.0f); }

	//if (SceneChangeFlag == true) { DebugText::GetInstance()->Print("SceneChangeF:true", 300, 200, 3.0f);
	//}
	//else { DebugText::GetInstance()->Print("SceneChangeF:false", 300, 200, 3.0f); }

}
