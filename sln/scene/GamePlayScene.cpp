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
#include "Pause.h"

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
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	//Model* model_3 = Model::LoadFromOBJ("chr_sword");
	//------3dオブジェクト生成------//
	object3d_1.reset(Object3d::Create());
	obj_worlddome.reset(Object3d::Create());
	obj_sword.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_tunnel.reset(Object3d::Create());
	//------3dオブジェクトに3dモデルを紐づける------//
	object3d_1->SetModel(model_1.get());
	obj_worlddome->SetModel(mod_worlddome.get());
	obj_sword->SetModel(mod_sword.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_tunnel->SetModel(mod_tunnel.get());
	//------object3dスケール------//
	object3d_1->SetScale({ 80.0f, 20.0f, 500.0f });
	obj_worlddome->SetScale({ 50.0f, 50.0f, 50.0f });
	obj_sword->SetScale({ 7.0f, 7.0f, 7.0f });
	obj_kaberight->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_kabeleft->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_tunnel->SetScale({ 2.0f, 2.0f, 2.0f });
	//------object3d位置------//
	object3d_1->SetPosition({ 0,-150,0 });
	obj_worlddome->SetPosition({ 0,200,150 });
	obj_sword->SetPosition({ 0,50,0 });
	obj_kaberight->SetPosition({ 310,-200,0 });
	obj_kabeleft->SetPosition({ -310,-200,0 });
	obj_tunnel->SetPosition({ 0,40,-170 });
	//------object回転------//
	obj_kaberight->SetRotation({ 0,90,0 });
	obj_kabeleft->SetRotation({ 0,-90,0 });
	obj_tunnel->SetRotation({ 0,-90,0 });

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
	for (std::unique_ptr<Boss>& boss : boss_)//ボス
	{
		boss = std::make_unique<Boss>();
		boss->Initialize();
		boss->SetModel(mod_enemy.get());
		boss->SetBulModel(mod_enemybullet.get());
		boss->SetAimBulModel(mod_enemybullet.get());
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

	Pause* pause = Pause::GetInstance();
	pause->Initialize();
	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	//SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");
	SpriteBase::GetInstance()->LoadTexture(13, L"Resources/sight.png");
	SpriteBase::GetInstance()->LoadTexture(14, L"Resources/Before_Boss.png");
	SpriteBase::GetInstance()->LoadTexture(15, L"Resources/GameReady.png");
	SpriteBase::GetInstance()->LoadTexture(16, L"Resources/GameGO!.png");
	SpriteBase::GetInstance()->LoadTexture(17, L"Resources/BlackWindow.png");
	SpriteBase::GetInstance()->LoadTexture(18, L"Resources/dame_ef.png");

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	//sp_guide.reset(Sprite::Create(2, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_sight.reset(Sprite::Create(13, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_beforeboss.reset(Sprite::Create(14, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_ready.reset(Sprite::Create(15, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_ready_go.reset(Sprite::Create(16, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_blackwindow.reset(Sprite::Create(17, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_dame_ef.reset(Sprite::Create(18, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);

	charParameters->Initialize();

	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });
	//sp_guide->SetPosition({ 0,0,0 });
	//sp_sight->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2,0 });
	sp_sight->SetPosition({ 0,0,0 });

	//スプライトカラー
	sp_blackwindow->SetColor({ 1, 1, 1, 0 });

	//---------スプライトサイズ---------//
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});

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
	charParameters->SetNowBoHp({ charParameters->GetboMaxHp() });//ボス体力
	charParameters->SetNowpHp({ charParameters->GetpMaxHp() });//自機体力

	//今あるパーティクルを削除する
	ParticleManager::GetInstance()->DeleteParticles();

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

	switch (beforeBossPattern_)
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
	if (BBPaternCount == 2 && beforeBossPattern_ == BeforeBossPattern::inc)
	{
		BeforeBossAppearFlag = true;
		BeforeBossAppearNow = true;
	}

	sp_beforeboss->SetColor(SP_BossWarning);

	//char tmp[32]{};
	//sprintf_s(tmp, 32, "%2.f", SP_BossWarning.w);
	//DebugText::GetInstance()->Print(tmp, 430, 490, 3);
}
void GamePlayScene::BossDeathEfect()
{
	XMFLOAT4 color = sp_blackwindow->GetColor();
	color.w += colordec;
	sp_blackwindow->SetColor(color);

	if (boss_.front()->GetPosition().y < object3d_1->GetPosition().y)
	{
		boss_.front()->SetAlive(false);
	}

	sp_blackwindow->Update();

	//ボス撃破でクリア　Update内だと一瞬画面見えちゃうからここに
	if (!boss_.front()->GetAlive()) {
		GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGMやめ
		BaseScene* scene = new ClearScene();
		sceneManager_->SetNextScene(scene);
	}
}

void GamePlayScene::PlayerMove()
{
	//Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

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
		constexpr float moveSpeed = 3.f;

		if (cInput->DownMove()) {
			PlayerPos.y -= moveSpeed;
		}

		if (cInput->UpMove()) {
			PlayerPos.y += moveSpeed;
		}

		if (cInput->LeftMove()) {

			PlayerPos.x -= moveSpeed;

			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}
			isLMove = true;//左移動中
		}

		if (cInput->RightMove()) {

			PlayerPos.x += moveSpeed;

			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}
			isRMove = true;//右移動中
		}
	}
	else
	{
		isLMove = false;
		isRMove = false;
	}

	//入力ないとき戻す
	if (rotation.z > 0 && isLMove == false) {
		rotation.z -= 1.f;
	}
	if (rotation.z < 0 && isRMove == false) {
		rotation.z += 1.f;
	}

	player_->SetPosition(PlayerPos);
	player_->SetRotation(rotation);
}

void GamePlayScene::CoolTime()
{
	//Input* input = Input::GetInstance();
	XMFLOAT4 pDamCol = sp_dame_ef->GetColor();
	//くーーーーるたいむ仮　今は文字だけ
	if (pDamFlag == true) {

		//DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

		//画像薄くしてく
		pDamCol.w -= 0.03;
		//0より大きい間かつまだ一回もやってないとき
		if (pDamCol.w > 0 && DamEfRedFlag == false) {
			sp_dame_ef->Update();
		}
		else {
			//繰り返さないように
			DamEfRedFlag = true;
		}
	}
	else {
		//ダメージ終わったら赤のダメージ画像色戻す
		DamEfRedFlag = false;
		pDamCol.w = 1;
	}
	sp_dame_ef->SetColor(pDamCol);

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
	//constexpr float camLen = 64.f;
	// カメラの高さ
	//constexpr float camHeight = camLen * 10.5f;

	// 自機からカメラ注視点までの距離
	//constexpr float player2targetLen = camLen * 2.f;

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
	if (sEnemyMurdersNum >= BossTermsEMurdersNum && BeforeBossAppearFlag == true) {
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

					if (bo->GetisDeath())continue;

					// 当たったら消える
					if (Collision::CheckSphere2Sphere(pBulForm, enemyForm)) {
						XMFLOAT3 boPos = bo->GetPosition();

						pb->SetAlive(false);

						//bo->SetColor({ 1,0,0,1 });
						if ((NowBoHp - (pBulPow - BossDefense)) > 0) {
							ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
						}
						NowBoHp -= (pBulPow - BossDefense);
						charParameters->SetNowBoHp(NowBoHp);//ボスHPセット

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.4f, 0);

						if (NowBoHp <= 0) {
							GameSound::GetInstance()->PlayWave("bossdeath.wav", 0.3f, 0);
							bo->SetisDeath(true);
							//残っている雑魚敵はもういらない
							for (auto& bob : bo->GetBullets()) {//いる雑魚敵の分だけ
								bob->SetAlive(false);//消す
							}

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
						charParameters->SetispDam(true);
						charParameters->SetNowpHp(NowpHp);//プレイヤーHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
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
						charParameters->SetispDam(true);//自機くらい
						charParameters->SetNowpHp(NowpHp);//ボスHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						break;
					}

				}
			}
		}
	}
	//------------------------------↑当たり判定ZONE↑-----------------------------//
}

bool GamePlayScene::GameReady()
{
	XMFLOAT4 ReadyCol = sp_ready->GetColor();
	XMFLOAT4 GOCol = sp_ready_go->GetColor();
	XMFLOAT2 GOSize = sp_ready_go->GetSize();
	XMFLOAT3 GOPos = sp_ready_go->GetPosition();
	//プレイヤー側でレディー中はAttackしないようにする
	bool pReadyFlag = player_->GetReadyNow();

	if (ReadyCol.w > 0.0)
	{
		ReadyCol.w -= 0.005;
		sp_ready->SetColor({ ReadyCol });
		sp_ready->Update();
	}

	if (ReadyCol.w < 0.0) {
		ready_GOFlag = true;
		GOCol.w -= 0.01;
		GOSize.x += 7;
		GOSize.y += 7;
		GOPos.x -= 3.2;
		GOPos.y -= 3.2;
		sp_ready_go->SetSize({ GOSize });
		sp_ready_go->TransferVertexBuffer();
		sp_ready_go->SetColor({ GOCol });
		sp_ready_go->SetPosition({ GOPos });
		sp_ready_go->Update();
	}

	if (GOCol.w < 0.0) {
		//アタック開始してよき
		pReadyFlag = false;
		player_->SetReadyNow(pReadyFlag);

		return false;
	}

	return true;
}

void GamePlayScene::Update()
{/*
	XMFLOAT4 pColor = player_->GetColor();
	pColor = { 1,0,0,1 };
	player_->SetColor(pColor);*/

	Pause* pause = Pause::GetInstance();

	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	//パッドトリガー
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));

	CharParameters* charParameters = CharParameters::GetInstance();

	if (pause->WaitKey0 < 10&& pause->GetPauseFlag() == false) {
		pause->WaitKey0++;//ポーズから入力待つ。1フレで開いて閉じちゃうから2回押した的な感じになっちゃう
	}
	if (pause->WaitKey0 >= 2) {
		if (charParameters->GetNowpHp() > 0 && charParameters->GetNowBoHp() > 0) {
			if (cInput->PauseOpenClose() && (GameReady() == false)&& pause->GetPauseFlag() == false) {
				pause->EveryInit();
				GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
				pause->SetPauseFlag(true);
			}
		}
	}
	if (pause->GetPauseFlag() == true) {
		
		pause->PauseNow();
		UpdateMouse();//ポーズしてるときもマウス更新　元はPause関数内

		if (pause->GetSceneChangeTitleFlag() == true) {
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();//振動
			// 音声停止
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");
			//シーン切り替え
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}

	}

	//ポーズでないとき～
	//この外はポーズ中も実行
	if (pause->GetPauseFlag() == false)
	{

		Input* input = Input::GetInstance();
		DxBase* dxBase = DxBase::GetInstance();

		CharParameters* charParameters = CharParameters::GetInstance();
		float NowBoHp = charParameters->GetNowBoHp();//現在のぼすHP取得
		float BossDefense = boss_.front()->GetBossDefense();//ボス防御力取得
		float NowpHp = charParameters->GetNowpHp();//自機体力取得

		//キー操作押している間

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

		//自機のHPバー
		charParameters->pHpSizeChange();

		charParameters->BarGetDislodged();

		//天球回転
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

		if (player_->GetPHpLessThan0() == false)
		{
			//プレイヤー移動-上に書くと移動かくつかない
			PlayerMove();
		}

		DrawUI();
		//パッド右スティックカメラ視点
		PadStickCamera();

		// マウス情報の更新
		UpdateMouse();
		// カメラの更新
		camera->Update();
		UpdateCamera();

		if (pRotDef == false) { //一度だけ
			input->PadVibrationDef();
			player_->SetRotation({});
			pRotDef = true;
		}

		//3dobjUPDATE
		object3d_1->Update();
		obj_worlddome->Update();
		//obj_sword->Update();
		obj_kaberight->Update();
		obj_kabeleft->Update();

		//スプライト更新
		sprite_back->Update();
		//sp_guide->Update();
		charParameters->pHpUpdate();

		pause->SpUpdate();

		player_->Update();

		if (GameReady() == false)
		{
			PlayTimer();

			//if (TriggerR) {//デバック用　適当に　いつかは消す
			//	camera->SetTarget({  });
			//	camera->SetEye({  });
			//	player_->SetAlive(true);
			//	NowpHp = pMaxHp;
			//	boss_.front()->SetAlive(true);
			//	NowBoHp = boMaxHp;
			//	sEnemyMurdersNum = 0;
			//	BossEnemyAdvent = false;

			//	// カメラreセット
			//	//Object3d::SetCamera(camera.get());
			//}

			//敵のHPバー
			if (BossEnemyAdvent == true)
			{
				charParameters->boHpSizeChange();
			}

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

			//------狙い弾↓
			//雑魚敵の撃つ弾がプレイヤーのいた場所に飛んでいく
			for (auto& se : smallEnemys_) {
				//ターゲット
				se->SetShotTag(player_.get());
			}
			for (auto& bo : boss_) {
				bo->SetShotTag(player_.get());
			}
			//------狙い弾↑
			
			//自機側で死亡確認したら消す
			if (player_->GetpDeath() == true) {
				GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.3f, 0);
				player_->SetAlive(false);
			}
			//----------------↓シーン切り替え関連↓----------------//
			//自機HP0でゲームオーバー
			if (!player_->GetAlive()) {
				GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGMやめ
				BaseScene* scene = new GameOver();
				sceneManager_->SetNextScene(scene);
			}
			//----------------↑シーン切り替え関連↑---------------//

			//くらったらクールタイム
			CoolTime();

			if (player_->GetPHpLessThan0() == false)
			{
				CollisionAll();
			}

			// パーティクル更新
			ParticleManager::GetInstance()->Update();

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

						if (boss->GetisDeath() == true)
						{
							BossDeathEfect();
						}
					}
				}
			}

			// FBX Update
			//fbxObject_1->Update();

			//sp_sight->Update();
			sp_beforeboss->Update();
			//敵のHPバー
			if (BossEnemyAdvent == true && NowBoHp > 0)
			{
				charParameters->boHpUpdate();
			}

		}

	}//ここまでポーズしてないとき

	//-------常にデバテキ↓
	// 
	//Pause* pause = Pause::GetInstance();
	//if (pause->GetPauseFlag() == false) {
	//	DebugText::GetInstance()->Print("pause:false", 100, 440, 2);
	//}
	//else {
	//	DebugText::GetInstance()->Print("pause:true", 100, 440, 2);
	//}

	//obj_tunnel->Update();

}

void GamePlayScene::Draw()
{
	Pause* pause = Pause::GetInstance();

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
	//obj_tunnel->Draw();

	//自キャラ描画
	player_->Draw();

	//smallEnemy_->Draw();

	// FBX3dオブジェクト描画
	//fbxObject_1->Draw(cmdList);

	// パーティクル描画
	DxBase* dxBase = DxBase::GetInstance();
	ParticleManager::GetInstance()->Draw(dxBase->GetCmdList());

	float NowBoHp = charParameters->GetNowBoHp();//現在のぼすHP取得

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
	if (pause->GetPauseFlag() == false)
	{
		//sp_guide->Draw();
		charParameters->pHpDraw();
		pause->SpOpenPauseDraw();
		//sp_sight->Draw();
	}
	if (pause->GetPauseFlag() == true) {
		pause->SpFlagTrueNowDraw();

	}
	else if (BossEnemyAdvent == true && NowBoHp > 0) {
		charParameters->boHpDraw();
	}//ボス戦時のみ表示

	if (pause->GetOpWindOpenFlag() == true) { pause->SpOperWindDraw(); }

	//ボス戦前 ポーズ中は見せない
	if (BeforeBossAppearNow == true && pause->GetPauseFlag() == false)
	{
		sp_beforeboss->Draw();
	}

	//開始前中のみ
	if (GameReady() == true)
	{
		sp_ready->Draw();
		if (ready_GOFlag == true) { sp_ready_go->Draw(); };
	}

	for (auto& bo : boss_) {
		if (bo->GetisDeath() == true) {
			sp_blackwindow->Draw();
		}
	}

	if (pDamFlag == true) {
		sp_dame_ef->Draw();
	}
	//向こうでダメージくらい状態解除したらこっちでも同様
	if (charParameters->GetispDam() == false) {
		pDamFlag = false;
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

	//{//振動時間
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%d", pShakeTimer_);
	//	DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	//}

	charParameters->DrawUI();
}

void GamePlayScene::PlayTimer()
{
	Pause* pause = Pause::GetInstance();
	//時間計測
	{
		Timer* timer = Timer::GetInstance();
		if (pause->GetPauseFlag() == false)
		{
			timer->TimerPlay();
		}
		char tmp[32]{};
		sprintf_s(tmp, 32, "NowTime : %2.f", timer->time);
		DebugText::GetInstance()->Print(tmp, 150, 220, 1);
	}
}