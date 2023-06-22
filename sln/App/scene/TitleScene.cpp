﻿#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "CharParameters.h"
#include "GameUtility.h"
#include "PostEffect.h"

using namespace DirectX;

namespace {
	constexpr auto eyeStartPos = XMFLOAT3(10 - 90, 160, -2000);
	constexpr auto eyeEndPos = XMFLOAT3(10, 160, -2000);
}

void TitleScene::Initialize()
{
#pragma region 描画初期化処理

	WinApp* winApp = WinApp::GetInstance();

	CharParameters* charParameters = CharParameters::GetInstance();
	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	camera.reset(new CameraTracking());
	Object3d::SetCamera(camera.get());

	////---objからモデルデータ読み込み---
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	mod_player.reset(Model::LoadFromOBJ("player"));
	mod_kaberight.reset(Model::LoadFromOBJ("Rkabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_logo.reset(Model::LoadFromOBJ("STRIKER_Logo"));
	mod_groundBottom.reset(Model::LoadFromOBJ("ground_bottom"));

	obj_ground.emplace("ground_gre", Object3d::Create());
	obj_ground.emplace("ground_mag", Object3d::Create());

	for (auto& i : obj_ground) {
		auto& model = mod_ground.emplace(i.first, Model::LoadFromOBJ(i.first)).first;
		constexpr float tilingNum = 16.f;
		model->second->SetTiling({ tilingNum, tilingNum });
		i.second->SetModel(mod_ground.at(i.first).get());
		i.second->SetScale({ 10000.0f, 1.0f, 10000.0f });
	}
	obj_ground.at("ground_mag")->SetPosition({ 0,-299,0 });

	////---3dオブジェクト生成---
	obj_tunnel.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_logo.reset(Object3d::Create());
	obj_groundBottom.reset(Object3d::Create());
	////---3dオブジェクトに3dモデルを紐づける---
	obj_tunnel->SetModel(mod_tunnel.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_logo->SetModel(mod_logo.get());
	obj_groundBottom->SetModel(mod_groundBottom.get());
	//------object3dスケール------//
	obj_tunnel->SetScale({ 100.0f, 40.0f, 40.0f });
	obj_kaberight->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_kabeleft->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_logo->SetScale({ 40.f,40.f,40.f });
	obj_groundBottom->SetScale({ 10000.0f, 10000.0f, 10000.0f });
	//------object3d位置------//
	obj_tunnel->SetPosition({ 0,40,-500 });
	obj_kaberight->SetPosition({ 490,340,-500 });
	obj_kabeleft->SetPosition({ -490,340,-500 });
	obj_logo->SetPosition({ 0,100,-1000 });
	obj_groundBottom->SetPosition({ 0,-190,0 });
	//------object回転
	obj_tunnel->SetRotation({ 0,-90,0 });
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

	charParameters->Initialize();
	//シーン遷移演出初期化
	sceneChangeDirection->Initialize();

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

	//スプライトポジション

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
	XMFLOAT3 rot = obj_logo->GetRotation();
	XMFLOAT3 pos = obj_logo->GetPosition();

	constexpr float RotSp = 0.03f;//回転速度
	constexpr float RotMax = 1.7f;//どこまで回転するか
	constexpr int PosYSp = 5;//上にずらす値
	constexpr int PosYMax = 300;//Y座標の最大値

	switch (logoPattern_) {
	case LogoPattern::def:
		if (pAppearFlag == false) {//登場が終わったら
			logoPattern_ = LogoPattern::rightRot;
		}
		break;

	case LogoPattern::rightRot:
		logoRotVel = -RotSp;
		if (rot.y <= -RotMax) {//最大値まで回転したら
			logoPattern_ = LogoPattern::leftRot;//次左回転
		}
		break;

	case LogoPattern::leftRot:
		logoRotVel = RotSp;
		if (rot.y >= RotMax) {//最大値まで回転したら
			logoPattern_ = LogoPattern::rightRot;//次右回転
		}
		break;

	case LogoPattern::beforeNextScene:
		logoRotVel = 0;
		pos.y = std::min(pos.y, (float)PosYMax);//Y座標はPosYMaxまでしかいけないように
		pos.y += PosYSp;
		break;
	}

	//シーンチェンジフラグ経ってなかったら上下移動
	if (!sceneChangeFlag) {
		pos.y += 0.2f * std::sin(time * XM_PI);
	}
	else {
		//シーンチェンジ開始したら
		logoPattern_ = LogoPattern::beforeNextScene;
	}

	rot.y += logoRotVel;
	obj_logo->SetRotation(rot);
	obj_logo->SetPosition(pos);

	time = frame++ / 60.f;
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

	LogoMove();//ロゴの動き

	obj_tunnel->Update();
	player_->Update();
	obj_kaberight->Update();
	obj_kabeleft->Update();
	obj_logo->Update();
	obj_groundBottom->Update();
	{
		float num = std::sin((float)time * swingSp) * swingDist;
		//地面の数だけ
		for (auto& i : obj_ground) {
			XMFLOAT3 pos = i.second->GetPosition();
			pos.y = posDef + num;//初期位置＋揺らす値
			i.second->SetPosition(pos);
			num = -num;//二枚目は逆に揺らす

			i.second->Update();
		}
	}

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
	player_->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();
	obj_logo->Draw();
	obj_groundBottom->Draw();
	for (auto& i : obj_ground) {
		i.second->Draw();
	}

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