#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "GameUtility.h"
#include "PostEffect.h"
#include "Field.h"
#include "Easing.h"
#include <yaml/Yaml.hpp>

using namespace DirectX;

namespace {
	constexpr auto eyeStartPos = XMFLOAT3(10 - 90, 160, -2000);
	constexpr auto eyeEndPos = XMFLOAT3(10, 160, -2000);
}

void TitleScene::Initialize()
{
	//ymlデータ
	{
		Yaml::Node root;
		try
		{
			Yaml::Parse(root, "Resources/charDataFile/title.yml");
		}
		catch (...)
		{
			throw;
		}

		RotSp = root["RotSp"].As<float>();
		RotMax = root["RotMax"].As<float>();
		PosYSp = root["PosYSp"].As<int16_t>();
		PosYMax = root["PosYMax"].As<int16_t>();
		vibCount = root["vibCount"].As<int16_t>();
		sceneChangeVibCount = root["sceneChangeVibCount"].As<int16_t>();
		toStartFrameDef = root["toStartFrameDef"].As<uint32_t>();
		toStartFrame = toStartFrameDef;
		exitPosZ = root["exitPosZ"].As<float>();
		sceneChangeDirecPosZ = root["sceneChangeDirecPosZ"].As<float>();
		auto& playerInitPosNode = root["playerInitPos"];
		playerInitPos = {
			playerInitPosNode["x"].As<float>(),
			playerInitPosNode["y"].As<float>(),
			playerInitPosNode["z"].As<float>()
		};
		camEyeMoveSpX = root["camEyeMoveSpX"].As<float>();
		pApMoveFrameMax = root["pApMoveFrameMax"].As<float>();
		pExitMoveFrameMax = root["pExitMoveFrameMax"].As<float>();
	}
#pragma region 描画初期化処理

	WinApp* winApp = WinApp::GetInstance();

	//フィールド初期化
	Field* field = Field::GetInstance();
	field->Initialize();

	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();
	//シーン遷移演出初期化
	sceneChangeDirection->Initialize();

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	camera.reset(new CameraTracking());
	Object3d::SetCamera(camera.get());

	////---objからモデルデータ読み込み---
	mod_player.reset(Model::LoadFromOBJ("player"));
	mod_kaberight.reset(Model::LoadFromOBJ("Rkabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_logo.reset(Model::LoadFromOBJ("STRIKER_Logo"));

	////---3dオブジェクト生成---
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_logo.reset(Object3d::Create());
	////---3dオブジェクトに3dモデルを紐づける---
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_logo->SetModel(mod_logo.get());
	//------object3dスケール------//
	obj_kaberight->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_kabeleft->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_logo->SetScale({ 3.f,3.f,3.f });//40

	//------object3d位置------//
	obj_kaberight->SetPosition({ 490,340,-500 });
	obj_kabeleft->SetPosition({ -490,340,-500 });
	obj_logo->SetPosition({ 0,100,-1000 });
	//------object回転
	obj_kaberight->SetRotation({ 0,0,0 });
	obj_kabeleft->SetRotation({ 0,180,0 });

	//いろいろ生成
	player_.reset(new Player());
	//いろいろキャラ初期化
	player_->Initialize();
	player_->SetPosition({ playerInitPos });
	player_->SetModel(mod_player.get());

	player_->SetAtkPossible(false);//タイトルでは弾を打たない

	//自機登場演出
	apEndPPos = player_->GetPosition();
	apStartPPos = apEndPPos;
	apStartPPos.z -= 1200;//ここから自機の初期位置まで指定フレーム掛けて動く

	camera->SetTarget(player_->GetPosition());
	camera->SetEye(eyeStartPos);//ここにカメラをおいて、最初の演出で自機を追いかける

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/Title_oper.png");

	// スプライトの生成
	sp_titleoper.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	logoPattern = std::bind(&TitleScene::RightRot, this);//ロゴの動き

#pragma endregion 描画初期化処理
}

void TitleScene::Finalize()
{
	//	delete postEffect;
}

void TitleScene::PlayerStandby()
{
	XMFLOAT3 pos = player_->GetPosition();
	//登場後の自機座標(=初期値)にカメラを固定して自機だけ動かす
	camera->SetTarget(playerInitPos);
	pos.y += 0.005f * sinf(time * 5.f);//振れ幅＊sin(時＊揺れ速度

	player_->SetPosition(pos);
}

void TitleScene::PlayerAppear()
{
	if (pMoveFrame < pApMoveFrameMax) {//最大フレーム到達までやる
		float raito = (float)pMoveFrame / pApMoveFrameMax;
		pMoveFrame++;
		//タイトルの最初に自機が移動しながら登場
		player_->SetPosition(GameUtility::UtilLerp(apStartPPos, apEndPPos, raito));

		//カメラずらしながら
		camera->SetEye(GameUtility::UtilLerp(eyeStartPos, eyeEndPos, raito));

		camera->SetTarget(player_->GetPosition());
	}
	else {//最大フレーム後
		XMFLOAT3 pos = player_->GetPosition();
		pMoveFrame = pMoveFrameDef;//シーン切り替えないでも使うのでデフォルトに戻す
		exitEndPPos = { pos.x,pos.y,exitPosZ };//退場は指定Zまで行っておわる
		exitStartPPos = pos;//現在自機座標から退場始める
		pAppearFlag = false;//登場完了
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
		doorOpenFlag = true;
	}
	obj_kabeleft->SetPosition(LDoorPos);
	obj_kaberight->SetPosition(RDoorPos);
}
void TitleScene::NextScene()
{
	Input* input = Input::GetInstance();
	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();

	DoorOpen();//扉を開ける

	//指定時間だけ振動する
	if (--sceneChangeVibCount == 0) {
		input->PadVibrationDef();
	}

	{//指定時間で移動
		float raito = (float)pMoveFrame / pExitMoveFrameMax;
		pMoveFrame++;
		player_->SetPosition(GameUtility::UtilLerp(exitStartPPos, exitEndPPos, raito));
		//カメラが自機をみる
		camera->SetTarget(player_->GetPosition());
	}
	//自機がシーン遷移演出開始位置に到達したら
	if (player_->GetPosition().z >= sceneChangeDirecPosZ && hideTheScreenOnly == false) {
		sceneChangeDirection->hideTheScreenFlag = true;//画面隠す開始
		hideTheScreenOnly = true;
	}

	if (sceneChangeDirection->sceneChangeCompFlag)//シーン遷移完了したら
	{
		// 音声停止
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}

	//シーン遷移演出更新
	sceneChangeDirection->Update();
}

void TitleScene::ToStartSprite()
{
	if (toStartFrame > 0) {
		--toStartFrame;
	}
	else
	{//指定時間たったら
		constexpr float ColorWDec = 0.012f;//透明にしていく速度
		constexpr float Transparency = 0.5f;//最終的な透明度がどこまで行くか。ここまでいったらデフォ値に戻す
		XMFLOAT4 color = sp_titleoper->GetColor();

		color.w -= ColorWDec;
		if (color.w <= Transparency) {
			toStartFrame = toStartFrameDef;//またこの時間分まつ
			color.w = 1.f;//一番明るい状態
		}

		sp_titleoper->SetColor(color);
		sp_titleoper->TransferVertexBuffer();
		sp_titleoper->Update();
	}
}

void TitleScene::LogoMove()
{
	if (pAppearFlag == false) {//登場が終わったら
		logoPattern();
	}

	//シーンチェンジフラグ経ってなかったら上下移動
	if (!sceneChangeFlag) {
		XMFLOAT3 pos = obj_logo->GetPosition();
		pos.y += 0.2f * std::sin(time * XM_PI);
		obj_logo->SetPosition(pos);
	}
	else {
		//シーンチェンジ開始したら
		logoPattern = std::bind(&TitleScene::BeforeNextScene, this);
	}

	obj_logo->SetRotation({ obj_logo->GetRotation().x, obj_logo->GetRotation().y + logoRotVel, obj_logo->GetRotation().z });

	time = frame++ / 60.f;
}
void TitleScene::RightRot()
{
	XMFLOAT3 rot = obj_logo->GetRotation();
	logoRotVel = -RotSp;
	if (rot.y <= -RotMax) {//最大値まで回転したら
		logoPattern = std::bind(&TitleScene::LeftRot, this);
	}
	obj_logo->SetRotation(rot);
}

void TitleScene::LeftRot()
{
	XMFLOAT3 rot = obj_logo->GetRotation();
	logoRotVel = RotSp;
	if (rot.y >= RotMax) {//最大値まで回転したら
		logoPattern = std::bind(&TitleScene::RightRot, this);
	}
	obj_logo->SetRotation(rot);
}
void TitleScene::BeforeNextScene()
{
	XMFLOAT3 pos = obj_logo->GetPosition();
	logoRotVel = 0;
	pos.y = std::min(pos.y, (float)PosYMax);//Y座標はPosYMaxまでしかいけないように
	pos.y += PosYSp;
	obj_logo->SetPosition(pos);
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();
	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();

	//セレクトから振動少し続ける
	if (--vibCount <= 0) {
		input->PadVibrationDef();
	}

	if (pAppearFlag) {
		PlayerAppear();//自機登場
	}
	else {
		if (sceneChangeFlag) {
			sceneChangeDirection->sceneChangeDirectionFlag = true;
			NextScene();//チェンジ移動開始
		}
		else {	//登場完了して退場前
			if (cInput->Decision())     // スペースキーorEnterが押されていたら
			{
				GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
				sceneChangeFlag = true;//チェンジ移動フラグ立てる
				input->PadVibration();
			}

			PlayerStandby();//たいきもーしょん
			ToStartSprite();//ENTER押してね的な画像関係
		}
	}

	// todo テストコード
	if (input->PushKey(DIK_0)) {
		
		constexpr float sPos = 0.f;
		constexpr float ePos = -500.f;
		constexpr float totalT = 180.f;

		XMFLOAT3 pos = obj_logo->GetPosition();
		if (testTime<=totalT) {//指定時間で終了
			pos.x = Easing::EaseInOutSine((float)testTime, sPos, ePos, totalT);
			obj_logo->SetPosition(pos);
			testTime++;
		}
	}
	else if (input->PushKey(DIK_1)) {//座標と時間リセット
		XMFLOAT3 pos = obj_logo->GetPosition();
		obj_logo->SetPosition({ 0,100,-1000 });
		testTime = 0;
	}
	{//でばてき　現在位置
		char tmp[32]{};
		sprintf_s(tmp, 32, "%f", obj_logo->GetPosition().x);
		DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	}
	{//でばてき　現在位置
		char tmp[32]{};
		sprintf_s(tmp, 32, "%f", (float)testTime);
		DebugText::GetInstance()->Print(tmp, 430, 460, 3);
	}

	//フィールド更新
	Field* field = Field::GetInstance();
	field->Update();

	LogoMove();//ロゴの動き

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

	//フィールド描画
	Field* field = Field::GetInstance();
	field->Draw();

	//３DオブジェクトDraw
	player_->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();
	obj_logo->Draw();

	//3dオブジェ描画後処理
	Object3d::PostDraw();
}

void TitleScene::DrawUI()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//// スプライト描画

	if (pAppearFlag == false && sceneChangeFlag == false)//自機登場終わってるかつENTER押される前なら
	{
		sp_titleoper->Draw();//ENTERで開始するよ！画像
	}

	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();
	if (sceneChangeDirection->sceneChangeDirectionFlag) {//シーン遷移演出中なら
		sceneChangeDirection->Draw();//シーン遷移演出描画
	}
}