#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "DxBase.h"
#include "ClearScene.h"
#include "GameOver.h"
#include "FbxObject3d.h"
#include "Collision.h"

#include "safe_delete.h"

#include <DirectXMath.h>

using namespace DirectX;

void GamePlayScene::Initialize()
{
#pragma region 描画初期化処理

	camera.reset(new Camera(WinApp::window_width, WinApp::window_height));

	camera->SetTarget({ 0,50,-200 });
	camera->SetEye({ 0,48,-210 });

	//デバイスをセット
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());
	// カメラセット
	Object3d::SetCamera(camera.get());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	FbxObject3d::SetCamera(camera.get());

	//使う定義とか　仮おいとくね
	time = frame / 60.f;	// 60fps想定

	//------objからモデルデータ読み込み---
	model_1.reset(Model::LoadFromOBJ("ground"));
	mod_worlddome.reset(Model::LoadFromOBJ("skydome"));
	mod_sword.reset(Model::LoadFromOBJ("chr_sword"));
	mod_kaberight.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_smallenemy.reset(Model::LoadFromOBJ("smallenemy_kari"));
	mod_playerbullet.reset(Model::LoadFromOBJ("PlayerBul"));
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	mod_player.reset(Model::LoadFromOBJ("hiyoko"));
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
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
	obj_worlddome->SetScale({ 5.0f, 5.0f, 5.0f });
	obj_sword->SetScale({ 7.0f, 7.0f, 7.0f });
	obj_kaberight->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_kabeleft->SetScale({ 200.0f, 200.0f, 10.0f });
	//------object3d位置------//
	object3d_1->SetPosition({ 0,-150,0 });
	obj_worlddome->SetPosition({ 0,200,150 });
	obj_sword->SetPosition({ 0,50,0 });
	obj_kaberight->SetPosition({ 210,-200,0 });
	obj_kabeleft->SetPosition({ -210,-200,0 });
	//------object回転------//
	obj_kaberight->SetRotation({ 0,90,0 });
	obj_kabeleft->SetRotation({ 0,-90,0 });

	//いろいろ生成
	player_.reset( new Player());
	//smallEnemy_ = new SmallEnemy();
	//いろいろキャラ初期化
	player_->Initialize();
	player_->SetModel(mod_player.get());
	player_->SetPBulModel(mod_playerbullet.get());
	//smallEnemy_->Initialize();

	boss_.emplace_front();
	for (std::unique_ptr<Boss>& boss : boss_)
	{
		boss=std::make_unique<Boss>();
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
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav",0.4f, XAUDIO2_LOOP_INFINITE);
	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/HPbar.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/HPbar_waku.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/playerHPbar.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/playerHPbar_waku.png");

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_guide.reset(Sprite::Create(2, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbar.reset(Sprite::Create(3, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbarwaku.reset(Sprite::Create(4, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbar.reset(Sprite::Create(5, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbarwaku.reset(Sprite::Create(6, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);
	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });
	sp_guide->SetPosition({ 0,0,0 });
	sp_enemyhpbar->SetPosition({ 140,-80,0 });
	sp_enemyhpbarwaku->SetPosition({ 140,-80,0 });
	sp_playerhpbar->SetPosition({ -30,500,0 });
	sp_playerhpbarwaku->SetPosition({ -30,500,0 });

	//---------スプライトサイズ---------//
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});
	sp_playerhpbarwaku->size_.x = PlayerMaxHP;
	sp_playerhpbarwaku->TransferVertexBuffer();

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
#pragma endregion 描画初期化処理

	//int counter = 0; // アニメーションの経過時間カウンター
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
	constexpr float camHeight = camLen * 0.5f;

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
	//// 自機の視線ベクトル
	//const XMVECTOR look = XMVector3Normalize(player_->GetLookVec());
	//// 自機->カメラのベクトル
	//const XMVECTOR player2cam = XMVectorAdd(XMVectorScale(look, -camLen),
	//	XMVectorSet(0, camHeight, 0, 1));

	//// カメラの位置
	//{
	//	const XMVECTOR pos = XMVectorAdd(player_->GetPosVec(), player2cam);

	//	XMFLOAT3 camPos{};
	//	XMStoreFloat3(&camPos, pos);

	//	camera->SetEye(camPos);
	//}
	//// 注視点設定
	//{
	//	const XMVECTOR targetPos = XMVectorAdd(XMVectorScale(look, player2targetLen),
	//		player_->GetPosVec());
	//	XMFLOAT3 targetF3{};
	//	XMStoreFloat3(&targetF3, targetPos);

	//	camera->SetTarget(targetF3);
	//}
}

void GamePlayScene::CollisionAll()
{
	//------------------------------↓当たり判定ZONE↓-----------------------------//
//[自機の弾]と[ボス]の当たり判定
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
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

						pb->SetAlive(false);

						NowBossHP -= pBulPower;

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.5, 0);

						if (NowBossHP <= 0) {
							GameSound::GetInstance()->PlayWave("bossdeath.wav", 0.5, 0);
							bo->SetAlive(false);
						}

						break;
					}
				}
			}
			//ボスいればTRUE　消えたらFALSE　いないとENPTY
			if (!boss_.empty())
			{
				if (boss_.front()->GetAlive()) {
					DebugText::GetInstance()->Print("TRUE", 200, 190, 2);
				}
				else {
					DebugText::GetInstance()->Print("FALSE", 200, 190, 2);
				}
			}
			else {
				DebugText::GetInstance()->Print("empty", 200, 190, 2);
			}
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
				smallenemyForm.radius = se->GetScale().x;

				// 当たったら消える
				if (Collision::CheckSphere2Sphere(pBulForm, smallenemyForm)) {
					GameSound::GetInstance()->PlayWave("se_baaan1.wav", 0.4f, 0);
					sEnemyMurdersNum++;//撃破数
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

	//[自機]と[敵弾]の当たり判定
	//XMFLOAT3 pPosMem{};//プレイヤー元座標保存　揺れに使う予定
	//XMFLOAT3 pos=player_->GetPosition();
	//if(pos.y==0){ DebugText::GetInstance()->Print("0niiru", 200, 350, 3);}//0に戻ったら表示目安
	{

		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z;

		if (player_->GetAlive()) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& eb : bo->GetBullets()) {
					Sphere eBulForm;
					eBulForm.center = XMLoadFloat3(&eb->GetPosition());
					eBulForm.radius = eb->GetScale().z;

					if (Collision::CheckSphere2Sphere(playerForm, eBulForm)) {

						//pPosMem = player_->GetPosition();//プレイヤー元座標保存　揺れに使う予定
						pDamFlag = true;
						NowPlayerHP -= eBulPower;//自機ダメージ

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.5, 0);
						eb->SetAlive(false);
						if (NowPlayerHP <= 0) {//HP0で死亡
							GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.5, 0);
							player_->SetAlive(false);
						}
						break;
					}

				}
			}
		}


	}
	//if(pPosMem.x==0){ DebugText::GetInstance()->Print("posMem is 0", 200, 390, 3); }//posmemに０はいってたらおせーて
	//------------------------------↑当たり判定ZONE↑-----------------------------//
}


void GamePlayScene::Update()
{

	Input* input = Input::GetInstance();
	DxBase* dxBase = DxBase::GetInstance();

	//キー操作押している間
	// 座標操作
	const bool inputUp = input->PushKey(DIK_UP);
	const bool inputDown = input->PushKey(DIK_DOWN);
	const bool inputRight = input->PushKey(DIK_RIGHT);
	const bool inputLeft = input->PushKey(DIK_LEFT);

	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	const bool inputT = input->PushKey(DIK_T);
	const bool inputE = input->PushKey(DIK_E);

	const bool inputI = input->PushKey(DIK_I);
	const bool inputK = input->PushKey(DIK_K);
	const bool inputJ = input->PushKey(DIK_J);
	const bool inputL = input->PushKey(DIK_L);
	//押した瞬間
	const bool TriggerM = input->TriggerKey(DIK_M);
	const bool TriggerE = input->TriggerKey(DIK_E);
	const bool TriggerR = input->TriggerKey(DIK_R);
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	const bool Trigger2 = input->TriggerKey(DIK_2);
	const bool TriggerESC = input->TriggerKey(DIK_ESCAPE);
	//パッド押している間
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	//--------------------↓移動制限

	const float CameraTagMoveLimX = 190;
	const float CameraEyeMoveLimX = 190;

	const float CameraMaxTagMoveLimY = 90;//Y最大タゲ　下にどれだけ行けるかなんか逆
	const float CameraMaxEyeMoveLimY = 92;//Y最大アイ
	const float CameraMinTagMoveLimY = 210;//Y最小タゲ　上にどれだけ行けるかなんか逆
	const float CameraMinEyeMoveLimY = 208;//Y最小アイ

	const float CameraMaxTagMoveLimZ = 320;//後最大タゲ
	const float CameraMaxEyeMoveLimZ = 330;//後最大アイ
	const float CameraMinTagMoveLimZ = 170;//前最小タゲ
	const float CameraMinEyeMoveLimZ = 160;//前最小アイ

			//------↓ターゲット
	XMFLOAT3 target_moved = camera->GetTarget();
	target_moved.x = max(target_moved.x, -CameraTagMoveLimX);
	target_moved.x = min(target_moved.x, +CameraTagMoveLimX);
	target_moved.y = max(target_moved.y, -CameraMaxTagMoveLimY); //下にどれだけ行けるかなんか逆
	target_moved.y = min(target_moved.y, +CameraMinTagMoveLimY);
	target_moved.z = max(target_moved.z, -CameraMaxTagMoveLimZ);
	target_moved.z = min(target_moved.z, +CameraMinTagMoveLimZ);//前
	camera->SetTarget(target_moved);
	//------↑ターゲット
	//------↓め！
	XMFLOAT3 eye_moved = camera->GetEye();
	eye_moved.x = max(eye_moved.x, -CameraEyeMoveLimX);
	eye_moved.x = min(eye_moved.x, +CameraEyeMoveLimX);
	eye_moved.y = max(eye_moved.y, -CameraMaxEyeMoveLimY); //上にどれだけ行けるかなんか逆
	eye_moved.y = min(eye_moved.y, +CameraMinEyeMoveLimY);
	eye_moved.z = max(eye_moved.z, -CameraMaxEyeMoveLimZ);//後
	eye_moved.z = min(eye_moved.z, +CameraMinEyeMoveLimZ);//前
	camera->SetEye(eye_moved);
	//------↑め！
//--------------------↑移動制限

	if (inputW || inputS || inputA || inputD || inputQ || inputZ || PadInputUP || PadInputDOWN || PadInputLEFT || PadInputRIGHT)
	{

		//------プレイヤーも同じ移動------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 2;

		if (inputS || PadInputDOWN) {
			// カメラをバックさせる
			camera->MoveEyeVector(XMFLOAT3(0, 0, -moveSpeed));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z - moveSpeed));

		}
		if (inputW || PadInputUP) {
			// カメラを前進させる
			camera->MoveEyeVector(XMFLOAT3(0, 0, +moveSpeed));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z + moveSpeed));

		}
		if (inputA || PadInputLEFT) {
			// カメラを左進させる
			camera->MoveEyeVector(XMFLOAT3(-moveSpeed, 0, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x - moveSpeed, camera->GetTarget().y, camera->GetTarget().z));

		}/*
		if(!inputA&&OldInputFlag_A==TRUE) {
			XMFLOAT3 rotation = obj_player->GetRotation();
			rotation.z = 0;
			obj_player->SetRotation(rotation);

			OldInputFlag_A = FALSE;
		}*/

		if (inputD || PadInputRIGHT) {
			// カメラを右進させる
			camera->MoveEyeVector(XMFLOAT3(moveSpeed, 0, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x + moveSpeed, camera->GetTarget().y, camera->GetTarget().z));

		}
		//else{ OldInputFlag = FALSE; }

		//if (OldInputFlag == TRUE) {

		//	XMFLOAT3 rotation = obj_player->GetRotation();
		//	rotation.z = 0;
		//	obj_player->SetRotation(rotation);

		//	OldInputFlag = FALSE;
		//}

		if (inputQ) {
			// カメラを上昇させる
			camera->MoveEyeVector(XMFLOAT3(0, moveSpeed, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y + moveSpeed, camera->GetTarget().z));

		}
		if (inputZ) {
			// カメラを下降させる
			camera->MoveEyeVector(XMFLOAT3(0, -moveSpeed, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y - moveSpeed, camera->GetTarget().z));
		}
	}

	if (TriggerR) {//デバック用　適当に　いつかは消す
		camera->SetTarget({ 0,50,-200 });
		camera->SetEye({ 0,48,-210 });
		player_->SetAlive(true);
		NowPlayerHP = PlayerMaxHP;
		boss_.front()->SetAlive(true);
		NowBossHP = BossMaxHP;
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

	//if (inputI || inputJ || inputK || inputL){
	//	// 現在の回転(ピッチ)を取得
	//	const float nowRotaP = player_->getLookVec().m128_f32[1];

	//	// 回転速度
	//	const float rotaSpeed = 1;

	//	// ----------
	//	// 入力
	//	// ----------

	//	if (inputI && playerRota.x + rotaSpeed < XM_PIDIV2) {
	//		playerRota.x += rotaSpeed;
	//	}
	//	else if (inputK && playerRota.x - rotaSpeed > -XM_PIDIV2) {
	//		playerRota.x -= rotaSpeed;
	//	}

	//	if (inputL) {
	//		playerRota.y += rotaSpeed;
	//	}
	//	else if (inputJ) {
	//		playerRota.y -= rotaSpeed;
	//	}

	//	// ----------
	//	// 視線を回転
	//	// ----------

	//	XMFLOAT3 rotaVec{};
	//	rotaVec.x += 175.f * dxBase->nearSin(playerRota.y) * dxBase->nearCos(playerRota.x);
	//	rotaVec.y += 175.f * dxBase->nearSin(playerRota.x);
	//	rotaVec.z += 175.f * dxBase->nearCos(playerRota.y) * dxBase->nearCos(playerRota.x);
	//}
	
	//くーーーーるたいむ仮　今は文字だけ
	if (pDamFlag == true) {
		if (--pShakeTimer_ >= 0) {//揺らす時間 0まで減らす			
			XMFLOAT3 pPosition = player_->GetPosition();
			DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);
			//pPosition.x = pPosMem.x+rand() % 5 - 2;
			//pPosition.y = pPosMem.y+rand() % 5 - 2;
			player_->SetPosition(pPosition);

			if(pShakeTimer_<=0){
				//player_->SetPosition({ pPosMem });//揺らした後元座標に戻したい
				pDamFlag = false;
			}//0なったらくらい状態解除
		}
	}
	else { pShakeTimer_ = pShakeTime; }
	if(pDamFlag==false){ DebugText::GetInstance()->Print("pdamflag=false", 200, 400, 3); }
	if (pDamFlag == true) { DebugText::GetInstance()->Print("pdamflag=true", 200, 400, 3); }

	//敵のHPバー
	if (BossEnemyAdvent == true)
	{
		//更新
		sp_enemyhpbar->Update();
		sp_enemyhpbarwaku->Update();
		for (int i = 0; i < 1; i++)
		{
			//サイズ変更
			sp_enemyhpbar->size_.x = NowBossHP;
			sp_enemyhpbar->TransferVertexBuffer();
		}
	}

	//自機のHPバー
	for (int i = 0; i < 1; i++)
	{
		sp_playerhpbar->size_.x = NowPlayerHP;
		sp_playerhpbar->TransferVertexBuffer();
	}
	//サイズ変更によるズレ
	if (NowPlayerHP <= 600 && BarPosControlOnlyOnceFlag1 == false) {
		XMFLOAT3 pHpBar = sp_playerhpbar->GetPosition();
		pHpBar.x += 50;
		sp_playerhpbar->SetPosition(pHpBar);
		BarPosControlOnlyOnceFlag1 = true;
	}
	if (NowPlayerHP <= 400 && BarPosControlOnlyOnceFlag2 == false) {
		XMFLOAT3 pHpBar = sp_playerhpbar->GetPosition();
		pHpBar.x += 30;
		sp_playerhpbar->SetPosition(pHpBar);
		BarPosControlOnlyOnceFlag2 = true;
	}

	//天球回転
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

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
	//雑魚敵更新
	if (BossEnemyAdvent == false)
	{
		for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
			smallEnemy->Update();
		}
	}

	//撃破数達成でボス戦
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		//ボス戦突入のお知らせです
		BossEnemyAdvent = true;
		DebugText::GetInstance()->Print("Boss HP", 500, 10, 2);
		//残っている雑魚敵はもういらない
		for (auto& se : smallEnemys_) {//いる雑魚敵の分だけ
			se->SetAlive(false);//消す
		}

		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Update();//敵更新
		}
	}

	//----------------↓シーン切り替え関連↓----------------//
	//敵撃破でクリア
	if(!boss_.front()->GetAlive()||Trigger2){
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

	DebugText::GetInstance()->Print("[PLAYSCENE]", 200, 100, 2);
	DebugText::GetInstance()->Print("[WASD&QZorGAMEPAD:STICK]MOVE", 200, 130, 2);
	DebugText::GetInstance()->Print("ALLOW:spriteMOVE", 200, 160, 2);
	DebugText::GetInstance()->Print("Player HP", 150, 610, 2);
	if (NowBossHP == 0) {
		DebugText::GetInstance()->Print("crushing!", 200, 230, 3);
	}
	if (player_->GetAlive()) {
		DebugText::GetInstance()->Print("Alive", 200, 270, 3);
	}
	else { DebugText::GetInstance()->Print("GameOver", 200, 270, 3); }
	DebugText::GetInstance()->Print("[2]ClearScene", 200, 210, 2);

	// マウス情報の更新
	UpdateMouse();
	camera->Update();
	// カメラの更新
	UpdateCamera();

	CollisionAll();

	//3dobjUPDATE
	object3d_1->Update();
	obj_worlddome->Update();
	obj_sword->Update();
	obj_kaberight->Update();
	obj_kabeleft->Update();

	// FBX Update
	//fbxObject_1->Update();

	//スプライト更新
	sprite_back->Update();
	sp_guide->Update();
	sp_playerhpbar->Update();
	sp_playerhpbarwaku->Update();

	player_->Update();
	//smallEnemy_->Update();

	//終了
	if (TriggerESC) {
		WM_DESTROY;//破棄されるウィンドに送信
		PostQuitMessage(0);//WM_DESTROYの応答、終了要求
	}
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();

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
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Draw();
		}
	}

	//3dオブジェ描画
	object3d_1->Draw();
	obj_worlddome->Draw();
	obj_sword->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();

	//自キャラ描画
	player_->Draw();
	//smallEnemy_->Draw();

	// FBX3dオブジェクト描画
	//fbxObject_1->Draw(cmdList);

	//パーティクル描画
	//particleMan->Draw(cmdList);

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
	sp_guide->Draw();
	sp_playerhpbar->Draw();
	sp_playerhpbarwaku->Draw();
	if (BossEnemyAdvent == true) { sp_enemyhpbar->Draw(); sp_enemyhpbarwaku->Draw(); }//ボス戦時のみ表示
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
   // sprite->Draw();

}

void GamePlayScene::DrawUI()
{
}
