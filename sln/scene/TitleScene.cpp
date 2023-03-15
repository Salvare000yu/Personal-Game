#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"
#include "CharParameters.h"

#include "Timer.h"

#include "PostEffect.h"

//PostEffect* postEffect = nullptr;

using namespace DirectX;

void TitleScene::Initialize()
{
#pragma region 描画初期化処理

	WinApp* winApp = WinApp::GetInstance();

	CharParameters* charParameters = CharParameters::GetInstance();

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	camera.reset(new CameraTracking());
	Object3d::SetCamera(camera.get());

	////---objからモデルデータ読み込み---
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	mod_ground.reset(Model::LoadFromOBJ("ground"));
	mod_player.reset(Model::LoadFromOBJ("player"));
	mod_kaberight.reset(Model::LoadFromOBJ("Rkabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_logo.reset(Model::LoadFromOBJ("STRIKER_Logo"));
	////---3dオブジェクト生成---
	obj_tunnel.reset(Object3d::Create());
	obj_ground.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_logo.reset(Object3d::Create());
	////---3dオブジェクトに3dモデルを紐づける---
	obj_tunnel->SetModel(mod_tunnel.get());
	obj_ground->SetModel(mod_ground.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_logo->SetModel(mod_logo.get());
	//------object3dスケール------//
	obj_tunnel->SetScale({ 100.0f, 40.0f, 40.0f });
	obj_ground->SetScale({ 80.0f, 20.0f, 500.0f });
	obj_kaberight->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_kabeleft->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_logo->SetScale({ 40.f,40.f,40.f });
	//------object3d位置------//
	obj_tunnel->SetPosition({ 0,40,-500 });
	obj_ground->SetPosition({ 0,-150,0 });
	obj_kaberight->SetPosition({ 490,340,-500 });
	obj_kabeleft->SetPosition({ -490,340,-500 });
	obj_logo->SetPosition({ 0,100,-1000 });
	//------object回転
	obj_tunnel->SetRotation({ 0,-90,0 });
	obj_kaberight->SetRotation({ 0,0,0 });
	obj_kabeleft->SetRotation({ 0,180,0 });
	obj_logo->SetRotation({ 0,0,0 });

	//いろいろ生成
	player_.reset(new Player());
	//いろいろキャラ初期化
	player_->Initialize();
	player_->SetPosition({ 0,150,-1950 });
	player_->SetModel(mod_player.get());

	player_->pAtkPossibleFlag = false;//タイトルでは弾を打たない

	//自機登場演出
	ApEndPPos = player_->GetPosition();
	ApStartPPos = ApEndPPos;
	ApStartPPos.z -= 500;//ここから自機の初期位置まで指定フレーム掛けて動く

	camera->SetTarget(player_->GetPosition());
	const float EyeXDef = 10;//最終位置
	const float EyeX = EyeXDef - (CamEyeMoveSpX * PApMoveFrameMax);//最終位置ー（自機登場時間＊ずらす値）　登場時間分ずらすから
	camera->SetEye({ EyeX,160,-2000 });//ここにカメラをおいて、最初の演出で自機を追いかける

	charParameters->Initialize();

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/Title_oper.png");

	// スプライトの生成
	sprite1.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_titleoper.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//スプライトポジション


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
	////window縦横取得したいとき使う
	//WinApp* winApp = WinApp::GetInstance();

	//const float spAccel = 1.05f;//加速

	////中心を取る
	//XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	//XMFLOAT2 NameTexSize = sp_gametitlename->GetTexSize();

	//NamePosXCenter = (winApp->window_width / 2) - (NameTexSize.x / 2);//画像左上にセットされるから中央を取る
	//NamePosYCenter = (winApp->window_height / 2) - (NameTexSize.y / 2);//〃;

	//if (NamePos.x > NamePosXCenter)
	//{
	//	NamePos.x -= sp;
	//	sp *= spAccel;
	//}
	//else {
	//	MoveStartFlag = false;
	//}

	//sp_gametitlename->SetPosition({ NamePos });
	////XMFLOAT2 NameSize = sp_gametitlename->GetSize();
	////NameSize.x--;
	////NameSize.y--;
	////sp_gametitlename->SetSize({NameSize});
	////sp_gametitlename->TransferVertexBuffer();

	////sp_gametitlename->SetPosition({ NamePosXCenter,NamePosYCenter,NamePos.z });
	//sp_gametitlename->Update();

}

void TitleScene::PlayerAppear()
{

	XMFLOAT3 pos = player_->GetPosition();

	if (PMoveFrame < PApMoveFrameMax) {//最大フレーム到達までやる

		float raito = (float)PMoveFrame / PApMoveFrameMax;
		PMoveFrame++;

		XMFLOAT3 pos{};
		pos.x = std::lerp(ApStartPPos.x, ApEndPPos.x, raito);
		pos.y = std::lerp(ApStartPPos.y, ApEndPPos.y, raito);
		pos.z = std::lerp(ApStartPPos.z, ApEndPPos.z, raito);
		player_->SetPosition(pos);

		XMFLOAT3 eyePos = camera->GetEye();
		eyePos.x += CamEyeMoveSpX;
		camera->SetEye(eyePos);

		camera->SetTarget(pos);
	}
	else {//最大フレーム後
		PMoveFrame = PMoveFrameDef;//シーン切り替えないでも使うのでデフォルトに戻す
		ExitEndPPos = { pos.x,pos.y,ExitPosZ };//退場は指定Zまで行っておわる
		ExitStartPPos = pos;//現在自機座標から退場始める
		PAppearFlag = false;//登場完了
	}

}
void TitleScene::DoorOpen()
{

	const int LDoorPosXRim = -2200;//左の壁開け終わる場所
	const float DoorMoveSp = 7.2f;//ドアが開く速度

	XMFLOAT3 LDoorPos = obj_kabeleft->GetPosition();
	XMFLOAT3 RDoorPos = obj_kaberight->GetPosition();

	//左の壁が一定行ったら終わり
	if (!(LDoorPos.x < LDoorPosXRim)) {
		LDoorPos.x -= DoorMoveSp;
		RDoorPos.x += DoorMoveSp;
	}
	else {
		DoorOpenFlag = true;
	}
	obj_kabeleft->SetPosition(LDoorPos);
	obj_kaberight->SetPosition(RDoorPos);
}
void TitleScene::SceneChange()
{
	//XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	//const float StartSpAccel = 1.05f;

	//NamePos.x -= StartSp;
	//StartSp *= StartSpAccel;
	//sp_gametitlename->SetPosition({ NamePos });

	Input* input = Input::GetInstance();

	if (PMoveFrame < PExitMoveFrameMax) {//退場用時間かけて退場する

		DoorOpen();//扉を開ける

		//指定時間だけ振動する
		if (--SceneChangeVibCount == 0) {
			input->PadVibrationDef();
		}

		float raito = (float)PMoveFrame / PExitMoveFrameMax;
		PMoveFrame++;

		XMFLOAT3 pos{};
		pos.x = std::lerp(ExitStartPPos.x, ExitEndPPos.x, raito);
		pos.y = std::lerp(ExitStartPPos.y, ExitEndPPos.y, raito);
		pos.z = std::lerp(ExitStartPPos.z, ExitEndPPos.z, raito);
		player_->SetPosition(pos);

		camera->SetTarget(pos);
	}
	else {//最大フレーム後
		// 音声停止
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}

	//sp_gametitlename->Update();
}

void TitleScene::ToStartSprite()
{
	XMFLOAT4 color = sp_titleoper->GetColor();

	ToStartFrame--;//透明でない時間
	ToStartFrame = max(ToStartFrame, 0);

	if (ToStartFrame <= 0) {
		color.w -= 0.012f;
	}

	if (color.w <= 0.5f) {
		ToStartFrame = ToStartFrameDef;//またこの時間分まつ
		color.w = 1.f;
	}

	sp_titleoper->SetColor(color);
	sp_titleoper->TransferVertexBuffer();
	sp_titleoper->Update();

	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", (float)ToStartFrame);
		DebugText::GetInstance()->Print(tmp, 300, 390, 3);
	}
}

void TitleScene::UpDown()
{
	//XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
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
	time = frame / 60;
	//NamePos.y += sinf(time * 6.f);
	frame++;
	//sp_gametitlename->SetPosition({ NamePos });
	//sp_gametitlename->Update();
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

	//sprite1->Update();
	if (PAppearFlag) {
		//BeforeUpdate(); 
		PlayerAppear();//自機登場
	}

	//登場完了して退場前
	if (PAppearFlag == false && SceneChangeFlag == false)
	{
		if ((cInput->DecisionByEnter()))     // スペースキーが押されていたら
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			SceneChangeFlag = true;//チェンジ移動フラグ立てる
			input->PadVibration();
		}

		ToStartSprite();
		UpDown();

		//postEffect->Update();
	}

	if (SceneChangeFlag) {
		SceneChange();//チェンジ移動開始
	}

	camera->SetTarget(player_->GetPosition());//カメラは自機を追う

	obj_tunnel->Update();
	obj_ground->Update();
	player_->Update();
	obj_kaberight->Update();
	obj_kabeleft->Update();
	obj_logo->Update();

	// カメラの更新
	camera->Update();
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
	player_->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();
	obj_logo->Draw();

	//3dオブジェ描画後処理
	Object3d::PostDraw();

	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//// スプライト描画
	//sprite1->Draw();

	//sp_gametitlename->Draw();

	if (PAppearFlag == false && SceneChangeFlag == false)//自機登場終わってるかつENTER押される前なら
	{
		sp_titleoper->Draw();//ENTERで開始するよ！画像
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

		//{
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%2.f", player_->GetPosition().z);
	//	DebugText::GetInstance()->Print(tmp, 300, 390, 3);
	//}
}
