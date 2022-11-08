#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Timer.h"
#include "DxBase.h"
#include "ParticleManager.h"
#include "CharParameters.h"

#include "TitleScene.h"
#include "ClearScene.h"
#include "GameOver.h"

#include "FbxObject3d.h"
#include "Collision.h"
#include "WinApp.h"

#include "../safe_delete.h"

#include "PostEffect.h"

#include <DirectXMath.h>

//using namespace DirectX;

void GamePlayScene::Initialize()
{

	//camera.reset(new Camera(WinApp::window_width, WinApp::window_height));
	camera.reset(new CameraTracking());

	//camera->SetTarget({ 0,50,-200 });
	//camera->SetEye({ 0,48,-210 });
	Object3d::SetCamera(camera.get());

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	//デバイスをセット
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());
	// カメラセット
	//Object3d::SetCamera(camera.get());

	//グラフィックスパイプライン生成
	//FbxObject3d::CreateGraphicsPipeline();
	//FbxObject3d::SetCamera(camera.get());

	//使う定義とか　仮おいとくね
	time = frame / 60.f;	// 60fps想定

	//------objからモデルデータ読み込み---
	model_1.reset(Model::LoadFromOBJ("ground"));
	mod_worlddome.reset(Model::LoadFromOBJ("skydome"));
	mod_sword.reset(Model::LoadFromOBJ("chr_sword"));
	mod_kaberight.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_smallenemy.reset(Model::LoadFromOBJ("SmallEnemy"));
	mod_playerbullet.reset(Model::LoadFromOBJ("bullet"));
	mod_enemybullet.reset(Model::LoadFromOBJ("enemyBul"));
	mod_player.reset(Model::LoadFromOBJ("player"));
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
	mod_firingline.reset(Model::LoadFromOBJ("firing_line"));
	//Model* model_3 = Model::LoadFromOBJ("chr_sword");
	//------3dオブジェクト生成------//
	object3d_1.reset(Object3d::Create());
	obj_worlddome.reset(Object3d::Create());
	obj_sword.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	//------3dオブジェクトに3dモデルを紐づける------//
	object3d_1->SetModel(model_1.get());
	obj_worlddome->SetModel(mod_worlddome.get());
	obj_sword->SetModel(mod_sword.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	//------object3dスケール------//
	object3d_1->SetScale({ 80.0f, 20.0f, 500.0f });
	obj_worlddome->SetScale({ 10.0f, 10.0f, 10.0f });
	obj_sword->SetScale({ 7.0f, 7.0f, 7.0f });
	obj_kaberight->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_kabeleft->SetScale({ 200.0f, 200.0f, 10.0f });
	//------object3d位置------//
	object3d_1->SetPosition({ 0,-150,0 });
	obj_worlddome->SetPosition({ 0,200,150 });
	obj_sword->SetPosition({ 0,50,0 });
	obj_kaberight->SetPosition({ 310,-200,0 });
	obj_kabeleft->SetPosition({ -310,-200,0 });
	//------object回転------//
	obj_kaberight->SetRotation({ 0,90,0 });
	obj_kabeleft->SetRotation({ 0,-90,0 });

	//いろいろ生成
	player_.reset(new Player());
	//いろいろキャラ初期化
	player_->Initialize();
	player_->SetModel(mod_player.get());
	player_->SetPBulModel(mod_playerbullet.get());
	player_->SetPFiringLine(mod_firingline.get());

	camera->SetTrackingTarget(player_.get());
	camera->SetTarget(player_->GetPosition());
	//XMFLOAT3 eye = player_->GetPosition();
	//eye.z -= 50;
	//eye.y += 10;
	//camera->SetEye(eye);

	boss_.emplace_front();
	for (std::unique_ptr<Boss>& boss : boss_)
	{
		boss = std::make_unique<Boss>();
		boss->Initialize();
		boss->SetModel(mod_enemy.get());
		boss->SetEBulModel(mod_enemybullet.get());
	}

	//fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//----------FBX オブジェクト生成とモデルのセット-----------//

	//fbxObject_1 = new FbxObject3d;

	//fbxObject_1->Initialize();

	//------fbxセット------//
	//fbxObject_1->SetModel(fbxModel_1);
	//------fbxスケール------//
	//fbxObject_1->SetScale({ 10.0f, 10.0f, 10.0f });
	//------fbx位置------//
	//fbxObject_1->SetPosition({ 0,24,100 });
	//fbxObject_1->PlayAnimation();

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("E_rhythmaze_128.wav");
	GameSound::GetInstance()->LoadWave("se_baaan1.wav");
	GameSound::GetInstance()->LoadWave("bossdam_1.wav");
	GameSound::GetInstance()->LoadWave("bossdeath.wav");
	GameSound::GetInstance()->LoadWave("playerdeath.wav");
	GameSound::GetInstance()->LoadWave("playerdam.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");
	GameSound::GetInstance()->LoadWave("personalgame_bosswarning.wav");
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav", 0.2f, XAUDIO2_LOOP_INFINITE);
	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];
	CharParameters* charParameters = CharParameters::GetInstance();
	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	//SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");
	SpriteBase::GetInstance()->LoadTexture(7, L"Resources/OpenPause.png");
	SpriteBase::GetInstance()->LoadTexture(8, L"Resources/pause.png");
	SpriteBase::GetInstance()->LoadTexture(9, L"Resources/continuation.png");
	SpriteBase::GetInstance()->LoadTexture(10, L"Resources/GoTitle.png");
	SpriteBase::GetInstance()->LoadTexture(11, L"Resources/Operation.png");
	SpriteBase::GetInstance()->LoadTexture(12, L"Resources/operation_wind.png");
	SpriteBase::GetInstance()->LoadTexture(13, L"Resources/sight.png");
	SpriteBase::GetInstance()->LoadTexture(14, L"Resources/Before_Boss.png");

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	//sp_guide.reset(Sprite::Create(2, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_openpause.reset(Sprite::Create(7, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_pause.reset(Sprite::Create(8, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_continuation.reset(Sprite::Create(9, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_gotitle.reset(Sprite::Create(10, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation.reset(Sprite::Create(11, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation_wind.reset(Sprite::Create(12, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_sight.reset(Sprite::Create(13, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_beforeboss.reset(Sprite::Create(14, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);

	charParameters->Initialize();

	//window縦横取得したいとき使う
	WinApp* winApp = WinApp::GetInstance();
	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });
	//sp_guide->SetPosition({ 0,0,0 });
	//sp_sight->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2,0 });
	sp_sight->SetPosition({ 0,0,0 });

	float gotitlePosY = winApp->window_width / 2;
	sp_continuation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 450,0 });//上
	sp_operation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 300,0 });//真ん中
	sp_gotitle->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 150 ,0 });//下
	sp_openpause->SetPosition({ 1050,600,0 });
	//---------スプライトサイズ---------//
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});
	sp_openpause->SetSize({ 210.f,130.f });
	sp_continuation->SetSize({ 300.f,100.f });
	sp_gotitle->SetSize({ 300.f,100.f });
	sp_operation->SetSize({ 300.f,100.f });

	sp_openpause->TransferVertexBuffer();
	sp_continuation->TransferVertexBuffer();
	sp_gotitle->TransferVertexBuffer();
	sp_operation->TransferVertexBuffer();

	// パーティクル初期化
	ParticleManager::GetInstance()->SetCamera(camera.get());

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

	//fbx モデル名指定してファイル読み込み
	//FbxLoader::GetInstance()->LoadModelFromFile(
	//	"cube"
	//);

	//int counter = 0; // アニメーションの経過時間カウンター

	//// スプライン曲線
	////posints = { start, start, p2, p3, end, end }
	//points.emplace_back(XMVectorSet(0, 0, 0, 0));//s
	//points.emplace_back(XMVectorSet(0, 0, 0, 0));//s
	//points.emplace_back(XMVectorSet(0, 0, 60, 0));
	//points.emplace_back(XMVectorSet(0, 70, 100, 0));
	//points.emplace_back(XMVectorSet(0, 30, 50, 0));//e
	//points.emplace_back(XMVectorSet(0, 0, 0, 0));//e
	////p1からスタート
	//splineStartIndex = 1;

	//時間リセット。タイトルに戻る度。
	Timer* timer = Timer::GetInstance();
	timer->TimerPlay(false);

	//パラメータ関連を初期化する
	charParameters->SetNowBoHp({ charParameters->GetboMaxHp()});//ボス体力
	charParameters->SetNowpHp({ charParameters->GetpMaxHp()});//自機体力
}

void GamePlayScene::Finalize()
{
	//safe_delete(fbxObject_1);
	//safe_delete(fbxModel_1);

	//自キャラ解放
	//delete player_;
	//delete smallEnemy_;
}

void GamePlayScene::SmallEnemyAppear()
{
	//キー入力使う
	//Input* input = Input::GetInstance();

	//雑魚敵生成
	std::unique_ptr<SmallEnemy> madeSmallEnemy = std::make_unique<SmallEnemy>();

	madeSmallEnemy->Initialize();

	madeSmallEnemy->SetModel(mod_smallenemy.get());
	for (std::unique_ptr<SmallEnemy>& se : smallEnemys_)
	{
		se->SetSEBulModel(mod_enemybullet.get());
	}
	//雑魚敵登録
	smallEnemys_.push_back(std::move(madeSmallEnemy));
}

void GamePlayScene::BeforeBossAppear()
{
	//演出中時のみtrue
	BeforeBossAppearNow = true;

	XMFLOAT4 SP_BossWarning = sp_beforeboss->GetColor();
	//SP_BossWarning.w -= 0.01;
	
	switch(beforeBossPattern_)
	{
	case BeforeBossPattern::def:
		if (AlertSoundFlag == true) {
			GameSound::GetInstance()->PlayWave("personalgame_bosswarning.wav", 0.3f, 0);
			AlertSoundFlag = false;
		}
		SP_BossWarning.w -= 0.03;
		if (SP_BossWarning.w < 0.0) {
			beforeBossPattern_ = BeforeBossPattern::inc;
		}
		break;
	case BeforeBossPattern::inc:
		SP_BossWarning.w += 0.03;
		if (SP_BossWarning.w > 1.0) {
			beforeBossPattern_ = BeforeBossPattern::dec;
			AlertSoundFlag = true;
			BBPaternCount++;//繰り返す回数
		}
			break;

	case BeforeBossPattern::dec:
		if (AlertSoundFlag == true) {
			GameSound::GetInstance()->PlayWave("personalgame_bosswarning.wav", 0.3f, 0);
			AlertSoundFlag = false;
		}
		SP_BossWarning.w -= 0.03;
		if (SP_BossWarning.w < 0.0) {
			beforeBossPattern_ = BeforeBossPattern::inc;
		}
			break;
	}

	//--繰り返す回数0~------消えてからボス戦へ
	if (BBPaternCount==2&& beforeBossPattern_==BeforeBossPattern::inc)
	{
		BeforeBossAppearFlag = true;
		BeforeBossAppearNow = true;
	}

	sp_beforeboss->SetColor(SP_BossWarning);

	//char tmp[32]{};
	//sprintf_s(tmp, 32, "%2.f", SP_BossWarning.w);
	//DebugText::GetInstance()->Print(tmp, 430, 490, 3);
}

void GamePlayScene::PlayerDeath()
{
	GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.3f, 0);
	player_->SetAlive(false);
}

void GamePlayScene::PlayerMove()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	//const bool inputW = input->PushKey(DIK_W);
	//const bool inputS = input->PushKey(DIK_S);
	//const bool inputA = input->PushKey(DIK_A);
	//const bool inputD = input->PushKey(DIK_D);
	//const bool inputQ = input->PushKey(DIK_Q);
	//const bool inputZ = input->PushKey(DIK_Z);
	//パッド押している間
	//const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	//const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	//const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	//const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	//----------↓移動制限
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 0;//下に行ける範囲
	const float PlayerMinMoveLimY = 200;//上に行ける範囲

	const float PlayerMaxMoveLimZ = 290;//後ろ
	const float PlayerMinMoveLimZ = 200;

	XMFLOAT3 PlayerPos = player_->GetPosition();
	XMFLOAT3 rotation = player_->GetRotation();
	PlayerPos.x = max(PlayerPos.x, -PlayerMoveLimX);
	PlayerPos.x = min(PlayerPos.x, +PlayerMoveLimX);
	PlayerPos.y = max(PlayerPos.y, -PlayerMaxMoveLimY);//下に行ける範囲
	PlayerPos.y = min(PlayerPos.y, +PlayerMinMoveLimY);//上に行ける範囲
	PlayerPos.z = max(PlayerPos.z, -PlayerMaxMoveLimZ);
	PlayerPos.z = min(PlayerPos.z, +PlayerMinMoveLimZ);
	player_->SetPosition(PlayerPos);
	//----------↑移動制限

	//------------------↓プレイヤー移動＆姿勢
	if (cInput->LeftMove() || cInput->RightMove() || cInput->UpMove() || cInput->DownMove())// inputQ || inputZ ||
	{
		//------プレイヤーも同じ移動------//
		//bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 2.f;

		if (cInput->DownMove()) { PlayerPos.y -= moveSpeed; }

		if (cInput->UpMove()) { PlayerPos.y += moveSpeed; }

		if (cInput->LeftMove()) {

			PlayerPos.x -= moveSpeed;

			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}

			//OldInputFlag = TRUE;
		}

		if (cInput->RightMove()) {

			PlayerPos.x += moveSpeed;

			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}

			//OldInputFlag = TRUE;
		}
		//if (inputQ) {

		//	PlayerPos.y += moveSpeed;
		//}

		//if (inputZ) {


		//	PlayerPos.y -= moveSpeed;
		//}
		player_->SetPosition(PlayerPos);
		player_->SetRotation(rotation);
	}
}

void GamePlayScene::CoolTime()
{
	Input* input = Input::GetInstance();
	//くーーーーるたいむ仮　今は文字だけ
	if (pDamFlag == true) {

		if (--pShakeTimer_ >= 0) {// 0まで減らす			
			//DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

			input->PadVibration();

			//pos揺らす
			XMFLOAT3 pos = player_->GetPosition();
			randShakeNow = 8 + 1;//a~b
			pos.x = pos.x + rand() % randShakeNow - 4;//a~bまでのrandShakeNowの最大値から半分を引いて負の数も含むように
			pos.y = pos.y + rand() % randShakeNow - 4;
			player_->SetPosition(pos);

			if (pShakeTimer_ <= 0) {
				input->PadVibrationDef();
				pDamFlag = false;
			}//0なったらくらい状態解除
		}
		else { pShakeTimer_ = pShakeTime; }
	}
	//if (pDamFlag == false) { DebugText::GetInstance()->Print("pdamflag=false", 100, 310, 2); }
	//if (pDamFlag == true) { DebugText::GetInstance()->Print("pdamflag=true", 100, 310, 2); }

}

void GamePlayScene::UpdateMouse()
{
	Input* input = Input::GetInstance();

	constexpr XMFLOAT2 centerPos = XMFLOAT2((float)WinApp::window_width / 2.f,
		(float)WinApp::window_height / 2.f);

	// 中心からの距
	cameraMoveVel.x = float(input->GetMousePos().x) - centerPos.x;
	cameraMoveVel.y = float(input->GetMousePos().y) - centerPos.y;

	input->SetMousePos((int)centerPos.x, (int)centerPos.y);
}

void GamePlayScene::UpdateCamera()
{
	// カメラの距離
	constexpr float camLen = 64.f;
	// カメラの高さ
	//constexpr float camHeight = camLen * 10.5f;

	// 自機からカメラ注視点までの距離
	constexpr float player2targetLen = camLen * 2.f;

	// 自機の視線ベクトル
	{
		//感度
		const float camMoveVel = 0.05f;

		XMFLOAT3 rota = player_->GetRotation();

		// マウスの横方向(X)の移動がカメラの縦方向(Y)の回転になる
		rota.x += cameraMoveVel.y * camMoveVel;
		// マウスの縦方向(Y)の移動がカメラの横方向(X)の回転になる
		rota.y += cameraMoveVel.x * camMoveVel;

		player_->SetRotation(rota);
	}
}

void GamePlayScene::PadStickCamera()
{
	//パッド右スティックでカメラ視点
	Input* input = Input::GetInstance();

	//感度
	const float PadCamMoveAmount = 0.5f;

	if (input->PushRightStickUp()) {
		XMFLOAT3 pRot = player_->GetRotation();
		pRot.x -= PadCamMoveAmount;
		player_->SetRotation(pRot);
	}
	if (input->PushRightStickDown()) {
		XMFLOAT3 pRot = player_->GetRotation();
		pRot.x += PadCamMoveAmount;
		player_->SetRotation(pRot);
	}
	if (input->PushRightStickRight()) {
		XMFLOAT3 pRot = player_->GetRotation();
		pRot.y += PadCamMoveAmount;
		player_->SetRotation(pRot);
	}
	if (input->PushRightStickLeft()) {
		XMFLOAT3 pRot = player_->GetRotation();
		pRot.y -= PadCamMoveAmount;
		player_->SetRotation(pRot);
	}
}

void GamePlayScene::CollisionAll()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	float NowBoHp = charParameters->GetNowBoHp();//現在のぼすHP取得
	float BossDefense = boss_.front()->GetBossDefense();//ボス防御力取得 先頭要素
	float NowpHp = charParameters->GetNowpHp();//自機体力取得
	float pBulPow = player_->GetpBulPow();//自機弾威力

	//------------------------------↓当たり判定ZONE↓-----------------------------//
	//[自機の弾]と[ボス]の当たり判定
	if (sEnemyMurdersNum >= BossTermsEMurdersNum&& BeforeBossAppearFlag == true) {
		{

			Sphere pBulForm;//球

			for (auto& pb : player_->GetBullets()) {
				if (!pb->GetAlive())continue;//死んでたらスキップ
				pBulForm.center = XMLoadFloat3(&pb->GetPosition());
				pBulForm.radius = pb->GetScale().x;

				// 衝突判定をする
				for (auto& bo : boss_) {
					if (!bo->GetAlive())continue;
					Sphere enemyForm;
					enemyForm.center = XMLoadFloat3(&bo->GetPosition());
					enemyForm.radius = bo->GetScale().x;

					// 当たったら消える
					if (Collision::CheckSphere2Sphere(pBulForm, enemyForm)) {
						XMFLOAT3 boPos = bo->GetPosition();
						
						pb->SetAlive(false);

						//bo->SetColor({ 1,0,0,1 });
						if ((NowBoHp - (pBulPow - BossDefense))>0) {
							ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
						}
						NowBoHp -= (pBulPow - BossDefense);
						charParameters->SetNowBoHp(NowBoHp);//ボスHPセット

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.4f, 0);

						if (NowBoHp <= 0) {
							GameSound::GetInstance()->PlayWave("bossdeath.wav", 0.3f, 0);
							bo->SetAlive(false);
						}

						break;
					}
				}
			}
			////ボスいればTRUE　消えたらFALSE　いないとENPTY
			//if (!boss_.empty())
			//{
			//	if (boss_.front()->GetAlive()) {
			//		DebugText::GetInstance()->Print("TRUE", 200, 190, 2);
			//	}
			//	else {
			//		DebugText::GetInstance()->Print("FALSE", 200, 190, 2);
			//	}
			//}
			//else {
			//	DebugText::GetInstance()->Print("empty", 200, 190, 2);
			//}
			// ボスを消す
			boss_.erase(std::remove_if(boss_.begin(), boss_.end(),
				[](const std::unique_ptr <Boss>& i) {return !i->GetAlive() && i->GetBullets().empty(); }), boss_.end());
		}
	}

	//[自機の弾]と[雑魚敵]当たり判定
	{

		Sphere pBulForm;

		for (auto& pb : player_->GetBullets()) {
			if (!pb->GetAlive())continue;
			pBulForm.center = XMLoadFloat3(&pb->GetPosition());
			pBulForm.radius = pb->GetScale().x;

			// 衝突判定をする
			for (auto& se : smallEnemys_) {
				if (!se->GetAlive())continue;
				Sphere smallenemyForm;
				smallenemyForm.center = XMLoadFloat3(&se->GetPosition());
				smallenemyForm.radius = se->GetScale().x + 5.f;//余裕を持たせる分＋

				// 当たったら消える
				if (Collision::CheckSphere2Sphere(pBulForm, smallenemyForm)) {
					GameSound::GetInstance()->PlayWave("se_baaan1.wav", 0.1f, 0);
					sEnemyMurdersNum++;//撃破数
					// パーティクルの発生
					XMFLOAT3 sePos = se->GetPosition();
					ParticleManager::GetInstance()->CreateParticle(sePos, 300, 80, 5);
					se->SetAlive(false);
					pb->SetAlive(false);
					break;
				}
			}
		}
	}

	//消滅フラグ立ったらその雑魚敵は死して拝せよ
	smallEnemys_.remove_if([](std::unique_ptr<SmallEnemy>& smallEnemy) {
		return !smallEnemy->GetAlive();
		});

	//[自機]と[ボス弾]の当たり判定
	{

		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;

		if (player_->GetAlive()) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& bob : bo->GetBullets()) {
					Sphere eBulForm;
					eBulForm.center = XMLoadFloat3(&bob->GetPosition());
					eBulForm.radius = bob->GetScale().z + 2.f;

					if (Collision::CheckSphere2Sphere(playerForm, eBulForm)) {

						pDamFlag = true;
						NowpHp -= eBulPower;//自機ダメージ
						charParameters->SetNowpHp(NowpHp);//プレイヤーHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
						if (NowpHp <= 0) {//HP0で死亡
							PlayerDeath();
						}
						break;
					}

				}
			}
		}


	}

	//[雑魚敵弾]と[自機]の当たり判定
	{
		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;

		if (player_->GetAlive()) {
			for (auto& se : smallEnemys_) {
				if (!se->GetAlive())continue;
				for (auto& seb : se->GetBullets()) {//seb 雑魚敵弾
					Sphere seBulForm;
					seBulForm.center = XMLoadFloat3(&seb->GetPosition());
					seBulForm.radius = seb->GetScale().z + 1;//余裕

					if (Collision::CheckSphere2Sphere(playerForm, seBulForm)) {
						float seBulPow = se->GetBulPow();//雑魚敵通常弾威力
						pDamFlag = true;
						NowpHp -= seBulPow;//自機ダメージ
						charParameters->SetNowpHp(NowpHp);//ボスHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						if (NowpHp <= 0) {//HP0で死亡
							PlayerDeath();
						}
						break;
					}

				}
			}
		}
	}
	//------------------------------↑当たり判定ZONE↑-----------------------------//
}

void GamePlayScene::PauseConti()
{
	Input* input = Input::GetInstance();
	//押した瞬間
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押している間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//選択中サイズでっかく
	sp_continuation->SetSize({ PauseSelectSize,100.f });
	sp_continuation->TransferVertexBuffer();

	if (TriggerDown || PadTriggerDown) {//1を次は選択
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 1;
	}
	if (TriggerUp || PadTriggerUp) {//上で2
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 2;
	}

	//継続
	if (TriggerEnter || PadTriggerA)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		PauseFlag = false;
	}

}
void GamePlayScene::PauseOper()
{
	Input* input = Input::GetInstance();
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押している間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//選択中サイズでっかく
	sp_operation->SetSize({ PauseSelectSize,100.f });
	sp_operation->TransferVertexBuffer();

	//操作説明開いてないときのみ
	if (OperWindOpenFlag == false)
	{
		if (TriggerDown || PadTriggerDown) {//下で2
			sp_operation->SetSize({ PauseSelectSizeDef,100.f });
			sp_operation->TransferVertexBuffer();
			PauseNowSelect = 2;
		}
		if (TriggerUp || PadTriggerUp) {//上で0
			sp_operation->SetSize({ PauseSelectSizeDef,100.f });
			sp_operation->TransferVertexBuffer();
			PauseNowSelect = 0;
		}
	}
	//操作説明画面開く
	if (TriggerEnter || PadTriggerA)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		OperWindOpenFlag = true;
		OperationWind();

		WaitKeyEnter++;
		if ((TriggerEnter || PadTriggerA) && WaitKeyEnter >= 2) {
			OperWindOpenFlag = false;
			WaitKeyEnter = 0;
		}
	}
}
void GamePlayScene::OperationWind()
{
	sp_operation_wind->Update();
}
void GamePlayScene::PauseGoTitle()
{
	Input* input = Input::GetInstance();
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//パッド押している間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//選択中サイズでっかく
	sp_gotitle->SetSize({ PauseSelectSize,100.f });
	sp_gotitle->TransferVertexBuffer();
	if (TriggerDown || PadTriggerDown) {//下で0
		sp_gotitle->SetSize({ PauseSelectSizeDef,100.f });
		sp_gotitle->TransferVertexBuffer();
		PauseNowSelect = 0;
	}
	if (TriggerUp || PadTriggerUp) {//上で1
		sp_gotitle->SetSize({ PauseSelectSizeDef,100.f });
		sp_gotitle->TransferVertexBuffer();
		PauseNowSelect = 1;
	}

	//タイトルへ戻る
	if (TriggerEnter || PadTriggerA)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		input->PadVibration();//振動
		// 音声停止
		GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

}

void GamePlayScene::Pause()
{

	Input* input = Input::GetInstance();
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	const bool Trigger0 = input->TriggerKey(DIK_0);
	//パッドトリガー
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));
	input->PadVibrationDef();
	// マウス情報の更新
	UpdateMouse();

	/*
	////選択中表示　デバッグ用
	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", PauseNowSelect);
		DebugText::GetInstance()->Print(tmp, 430, 460, 5);
	}*/

	//メンバ関数ポインタ対応した選択
	if (PauseNowSelect == 0) { pFunc = &GamePlayScene::PauseConti; }
	if (PauseNowSelect == 1) { pFunc = &GamePlayScene::PauseOper; }
	if (PauseNowSelect == 2) { pFunc = &GamePlayScene::PauseGoTitle; }

	//メンバ関数ポインタ呼び出し
	(this->*pFunc)();

	//閉じる
	WaitKey0++;//同じボタンでとじれるように
	//操作説明画面見てるときは押しても閉じない
	if (((Trigger0 || PadTriggerStart) && WaitKey0 >= 2) && OperWindOpenFlag == false) {
		PauseFlag = false;
		WaitKey0 = 0;
	}

}

void GamePlayScene::Update()
{

	Input* input = Input::GetInstance();
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	//パッドトリガー
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));

	if (Trigger0 || PadTriggerStart) {
		PauseFlag = true;
	}
	if (PauseFlag == true) {
		Pause();
	}

	//ポーズでないとき～
	//この外はポーズ中も実行
	if (PauseFlag == false)
	{

		Input* input = Input::GetInstance();
		DxBase* dxBase = DxBase::GetInstance();

		CharParameters* charParameters = CharParameters::GetInstance();
		float NowBoHp = charParameters->GetNowBoHp();//現在のぼすHP取得
		float BossDefense = boss_.front()->GetBossDefense();//ボス防御力取得
		float NowpHp = charParameters->GetNowpHp();//自機体力取得

		//キー操作押している間
		// 座標操作
		const bool inputUp = input->PushKey(DIK_UP);
		const bool inputDown = input->PushKey(DIK_DOWN);
		const bool inputRight = input->PushKey(DIK_RIGHT);
		const bool inputLeft = input->PushKey(DIK_LEFT);

		const bool inputT = input->PushKey(DIK_T);
		const bool inputE = input->PushKey(DIK_E);

		const bool inputI = input->PushKey(DIK_I);
		const bool inputK = input->PushKey(DIK_K);
		const bool inputJ = input->PushKey(DIK_J);
		const bool inputL = input->PushKey(DIK_L);

		const bool input3 = input->PushKey(DIK_3);
		//押した瞬間
		const bool TriggerM = input->TriggerKey(DIK_M);
		const bool TriggerE = input->TriggerKey(DIK_E);
		const bool TriggerR = input->TriggerKey(DIK_R);
		const bool Trigger2 = input->TriggerKey(DIK_2);

		float pMaxHp = charParameters->GetpMaxHp();
		float boMaxHp = charParameters->GetboMaxHp();
		if (TriggerR) {//デバック用　適当に　いつかは消す
			camera->SetTarget({  });
			camera->SetEye({  });
			player_->SetAlive(true);
			NowpHp = pMaxHp;
			boss_.front()->SetAlive(true);
			NowBoHp = boMaxHp;
			sEnemyMurdersNum = 0;
			BossEnemyAdvent = false;

			// カメラreセット
			//Object3d::SetCamera(camera.get());
		}
		
		////タゲ移動
		//if (inputUp || inputDown || inputRight || inputLeft)
		//{
		//	constexpr float tagmove = 5;
		//	if (inputUp) {
		//		XMFLOAT3 position = sp_guide->GetPosition();
		//		position.y -= tagmove;
		//		sp_guide->SetPosition({ position });
		//	}
		//	if (inputDown) {
		//		XMFLOAT3 position = sp_guide->GetPosition();
		//		position.y += tagmove;
		//		sp_guide->SetPosition({ position });
		//	}
		//	if (inputRight) {
		//		XMFLOAT3 position = sp_guide->GetPosition();
		//		position.x += tagmove;
		//		sp_guide->SetPosition({ position });
		//	}
		//	if (inputLeft) {
		//		XMFLOAT3 position = sp_guide->GetPosition();
		//		position.x -= tagmove;
		//		sp_guide->SetPosition({ position });
		//	}
		//}

		//敵のHPバー
		if (BossEnemyAdvent == true)
		{
			charParameters->boHpSizeChange();
		}

		//自機のHPバー
		charParameters->pHpSizeChange();

		charParameters->BarGetDislodged();

		//天球回転
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

		if (BossEnemyAdvent == false)
		{
			//時が満ちたら
			if (SEneAppCount == 0) {
				//雑魚敵来る
				SmallEnemyAppear();
				//再びカウントできるように初期化
				SEneAppCount = SEneAppInterval;
			}
		}
		//雑魚敵カウントをデクリメント
		SEneAppCount--;

		//----------------↓シーン切り替え関連↓----------------//
		//敵撃破でクリア
		if (!boss_.front()->GetAlive()) {
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGMやめ
			BaseScene* scene = new ClearScene();
			sceneManager_->SetNextScene(scene);
		}
		//自機HP0でゲームオーバー
		if (!player_->GetAlive()) {
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGMやめ
			BaseScene* scene = new GameOver();
			sceneManager_->SetNextScene(scene);
		}
		//----------------↑シーン切り替え関連↑---------------//

		//バックスプライト動
		//SPmove SPbackmoveobj;
		//for (int i = 0; i < 1; i++)
		//{
		//	XMFLOAT3 position = sprite_back->GetPosition();

		//	position.x += 5;
		//	if (position.x == 0) { position.x = -11400; }

		//	sprite_back->SetPosition(position);
		//}

		//くらったらクールタイム
		CoolTime();
		CollisionAll();
		DrawUI();
		//パッド右スティックカメラ視点
		PadStickCamera();

		// マウス情報の更新
		UpdateMouse();
		// カメラの更新
		camera->Update();
		UpdateCamera();
		// パーティクル更新
		ParticleManager::GetInstance()->Update();

		if (pRotDef == false) { //一度だけ
			input->PadVibrationDef();
			player_->SetRotation({});
			pRotDef = true;
		}

		//プレイヤー移動
		PlayerMove();

		//// スプライン曲線で移動
		//{
		//	frame++;
		//	float timeRate = (float)frame / 120.f;

		//	if (timeRate >= 1.0f)
		//	{
		//		if (splineStartIndex < points.size() - 3) {
		//			splineStartIndex++;
		//			timeRate -= 1.0f;
		//			frame = 0;
		//		}
		//		else
		//		{
		//			timeRate = 1.0f;
		//		}
		//	}

		//	// ベクターをフロートに変換
		//	XMFLOAT3 splineFloat;
		//	XMStoreFloat3(&splineFloat, SplinePosition(points, splineStartIndex, timeRate));

		//	player_->SetPosition(splineFloat);
		//}

		//3dobjUPDATE
		object3d_1->Update();
		obj_worlddome->Update();
		//obj_sword->Update();
		obj_kaberight->Update();
		obj_kabeleft->Update();
		//雑魚敵更新
		if (BossEnemyAdvent == false)
		{
			for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
				smallEnemy->Update();
			}
		}

		//撃破数達成でボス戦
		if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
			//残っている雑魚敵はもういらない
			for (auto& se : smallEnemys_) {//いる雑魚敵の分だけ
				se->SetAlive(false);//消す
			}

			if (BeforeBossAppearFlag == false)
			{
				BeforeBossAppear();
			}
			//ボス戦前の演出
			if (BeforeBossAppearFlag == true)
			{//演出終わったら
				//ボス戦突入のお知らせです
				BossEnemyAdvent = true;
				for (std::unique_ptr<Boss>& boss : boss_) {
					boss->Update();//敵更新
				}
			}
		}
		// FBX Update
		//fbxObject_1->Update();

		//スプライト更新
		sprite_back->Update();
		//sp_guide->Update();
		charParameters->pHpUpdate();
		sp_openpause->Update();
		sp_pause->Update();
		sp_continuation->Update();
		sp_gotitle->Update();
		sp_operation->Update();
		//sp_sight->Update();
		sp_beforeboss->Update();
		//敵のHPバー
		if (BossEnemyAdvent == true)
		{
			charParameters->boHpUpdate();
		}

		player_->Update();
	}//ここまでポーズしてないとき

}

void GamePlayScene::Draw()
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();

	CharParameters* charParameters = CharParameters::GetInstance();
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	//sprite_back->Draw();


	//3dオブジェ描画前処理
	Object3d::PreDraw(DxBase::GetInstance()->GetCmdList());

	//雑魚敵
	for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
		smallEnemy->Draw();
	}

	//敵描画
	if (sEnemyMurdersNum >= BossTermsEMurdersNum && BeforeBossAppearFlag == true) {
		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Draw();
		}
	}

	//3dオブジェ描画
	object3d_1->Draw();
	obj_worlddome->Draw();
	//obj_sword->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();

	//自キャラ描画
	player_->Draw();

	//smallEnemy_->Draw();

	// FBX3dオブジェクト描画
	//fbxObject_1->Draw(cmdList);

	// パーティクル描画
	DxBase* dxBase = DxBase::GetInstance();
	ParticleManager::GetInstance()->Draw(dxBase->GetCmdList());

	//3dオブジェ描画後処理
	Object3d::PostDraw();

	// ４．描画コマンドここから

	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//    DrawObject3d(&object3ds[i], dxBase->GetCmdList(), basicDescHeap.Get(), vbView, ibView,
	//        CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeap->GetGPUDescriptorHandleForHeapStart(), constantBufferNum, dxBase->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)),
	//        indices, _countof(indices));
	//}

	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();

	//---------------お手前スプライト描画
	if (PauseFlag == false)
	{
		//sp_guide->Draw();
		charParameters->pHpDraw();
		sp_openpause->Draw();
		//sp_sight->Draw();
	}
	if (PauseFlag == true) {
		sp_pause->Draw();
		sp_continuation->Draw();
		sp_gotitle->Draw();
		sp_operation->Draw();

	}
	else if (BossEnemyAdvent == true) { charParameters->boHpDraw(); }//ボス戦時のみ表示

	if (OperWindOpenFlag == true) { sp_operation_wind->Draw(); }

	//ボス戦前 ポーズ中は見せない
	if (BeforeBossAppearNow == true && PauseFlag == false)
	{
		sp_beforeboss->Draw();
	}

	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
   // sprite->Draw();

}

//XMVECTOR GamePlayScene::SplinePosition(const std::vector<XMVECTOR>& posints, size_t startIndex, float t)
//{
//
//	size_t n = posints.size() - 2;
//
//	if (startIndex > n)return posints[n];
//	if (startIndex < 1)return posints[1];
//
//	XMVECTOR p0 = posints[startIndex - 1];
//	XMVECTOR p1 = posints[startIndex];
//	XMVECTOR p2 = posints[startIndex + 1];
//	XMVECTOR p3 = posints[startIndex + 2];
//
//	//mt3スプライン曲線の考え方
//	XMVECTOR position = 0.5 * ((2 * p1 + (-p0 + p2) * t) +
//		(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
//		(-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
//
//	return position;
//}

void GamePlayScene::DrawUI()
{
	CharParameters* charParameters = CharParameters::GetInstance();
	//条件なし常に表示
	//DebugText::GetInstance()->Print("---PLAYSCENE---", 100, 70, 2);
	//DebugText::GetInstance()->Print("[LEFT CLICKorSPACEorPAD ZR] Firing", 100, 100, 2);
	//DebugText::GetInstance()->Print("[WASD&QZorGAMEPAD:STICK]PlayerMove", 100, 130, 2);
	//DebugText::GetInstance()->Print("[ALLOWorMOVE MOUSEorJ,K,L,I] PlayerRot", 100, 160, 2);
	//DebugText::GetInstance()->Print("[ESC] CLOSE WINDOW", 100, 190, 2);
	DebugText::GetInstance()->Print("Player HP", 150, 610, 2);
	//{
	////自機座標
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%2.f,%2.f,%2.f", player_->GetPosition().x, player_->GetPosition().y, player_->GetPosition().z);
	//	DebugText::GetInstance()->Print(tmp, 430, 220, 3);
	//}
	//if (NowBossHP == 0) {
	//	DebugText::GetInstance()->Print("crushing!", 100, 230, 3);
	//}
	//if (player_->GetAlive()) {
	//	DebugText::GetInstance()->Print("Alive", 100, 270, 3);
	//}
	//else { DebugText::GetInstance()->Print("GameOver", 100, 270, 3); }

	//時間計測
	{
		Timer* timer = Timer::GetInstance();
		if (PauseFlag == false)
		{
			timer->TimerPlay();
		}
		char tmp[32]{};
		sprintf_s(tmp, 32, "NowTime : %2.f", timer->time);
		DebugText::GetInstance()->Print(tmp, 150, 220, 3);
	}

	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {//ボス戦時のみ
		DebugText::GetInstance()->Print("Boss HP", 500, 10, 2);
		//DebugText::GetInstance()->Print("!!!Boss!!!", 100, 415, 3);
	}
	//else {//ボス戦じゃないときのみ表示
	//	//雑魚敵撃破数関連
	//	{
	//		DebugText::GetInstance()->Print("[BossTerms]", 100, 400, 2);

	//		char tmp[32]{};
	//		sprintf_s(tmp, 32, "%2.f", BossTermsEMurdersNum);
	//		DebugText::GetInstance()->Print(tmp, 300, 390, 3);
	//	}
	//	{
	//		DebugText::GetInstance()->Print("[Now DefeatedEnemy]", 100, 440, 2);

	//		char tmp[32]{};
	//		sprintf_s(tmp, 32, "%2.f", sEnemyMurdersNum);
	//		DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	//	}
	//}

	charParameters->DrawUI();
}
