#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "../DxBase.h"

#include "TitleScene.h"
#include "ClearScene.h"
#include "GameOver.h"

#include "FbxObject3d.h"
#include "Collision.h"
#include "WinApp.h"

#include "../safe_delete.h"

#include <DirectXMath.h>

using namespace DirectX;

void GamePlayScene::Initialize()
{
#pragma region 描画初期化処理

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
	mod_smallenemy.reset(Model::LoadFromOBJ("smallenemy_kari"));
	mod_playerbullet.reset(Model::LoadFromOBJ("bullet"));
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	mod_player.reset(Model::LoadFromOBJ("player"));
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
	obj_worlddome->SetScale({ 8.0f, 8.0f, 8.0f });
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

	camera->SetTrackingTarget(player_.get());
	camera->SetTarget(player_->GetPosition());
	XMFLOAT3 eye = player_->GetPosition();
	eye.z -= 50;
	eye.y += 10;
	camera->SetEye(eye);

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
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav", 0.2f, XAUDIO2_LOOP_INFINITE);
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
	SpriteBase::GetInstance()->LoadTexture(7, L"Resources/Miscellaneous Enemy Meter.png");
	SpriteBase::GetInstance()->LoadTexture(8, L"Resources/pause.png");
	SpriteBase::GetInstance()->LoadTexture(9, L"Resources/continuation.png");
	SpriteBase::GetInstance()->LoadTexture(10, L"Resources/GoTitle.png");
	SpriteBase::GetInstance()->LoadTexture(11, L"Resources/Operation.png");

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_guide.reset(Sprite::Create(2, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbar.reset(Sprite::Create(3, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbarwaku.reset(Sprite::Create(4, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbar.reset(Sprite::Create(5, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbarwaku.reset(Sprite::Create(6, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_semeter.reset(Sprite::Create(7, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_pause.reset(Sprite::Create(8, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_continuation.reset(Sprite::Create(9, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_gotitle.reset(Sprite::Create(10, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation.reset(Sprite::Create(11, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);

	//window縦横取得したいとき使う
	WinApp* winApp = WinApp::GetInstance();
	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });
	sp_guide->SetPosition({ 0,0,0 });
	sp_enemyhpbar->SetPosition({ 140,-80,0 });
	sp_enemyhpbarwaku->SetPosition({ 140,-80,0 });
	sp_playerhpbar->SetPosition({ -30,500,0 });
	sp_playerhpbarwaku->SetPosition({ -30,500,0 });
	sp_semeter->SetPosition({ 1170,620,0 });

	float gotitlePosY = winApp->window_width / 2;
	sp_continuation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 450,0 });//上
	sp_operation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 300,0 });//真ん中
	sp_gotitle->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 150 ,0 });//下

	//---------スプライトサイズ---------//
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});
	sp_playerhpbarwaku->size_.x = PlayerMaxHP;
	sp_semeter->SetSize({ 70.f,70.f });
	sp_continuation->SetSize({ 300.f,100.f });
	sp_gotitle->SetSize({ 300.f,100.f });
	sp_operation->SetSize({ 300.f,100.f });


	sp_playerhpbarwaku->TransferVertexBuffer();
	sp_semeter->TransferVertexBuffer();
	sp_continuation->TransferVertexBuffer();
	sp_gotitle->TransferVertexBuffer();
	sp_operation->TransferVertexBuffer();

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

void GamePlayScene::PlayerDeath()
{
	GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.3f, 0);
	player_->SetAlive(false);
}

void GamePlayScene::PlayerMove()
{
	Input* input = Input::GetInstance();
	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	//パッド押している間
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	//----------↓移動制限
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 100;//下に行ける範囲
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
	if (inputW || inputS || inputA || inputD || inputQ || inputZ || PadInputUP || PadInputDOWN || PadInputLEFT || PadInputRIGHT)
	{
		//------プレイヤーも同じ移動------//
		//bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 2.f;

		if ((inputS) || PadInputDOWN) { PlayerPos.y -= moveSpeed; }

		if ((inputW) || PadInputUP) { PlayerPos.y += moveSpeed; }

		if ((inputA) || PadInputLEFT) {

			PlayerPos.x -= moveSpeed;

			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}

			//OldInputFlag = TRUE;
		}

		if ((inputD) || PadInputRIGHT) {

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
	//くーーーーるたいむ仮　今は文字だけ
	if (pDamFlag == true) {
		if (--pShakeTimer_ >= 0) {// 0まで減らす			
			DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

			//pos揺らす
			XMFLOAT3 pos = player_->GetPosition();
			randShakeNow = 8 + 1;//a~b
			pos.x = pos.x + rand() % randShakeNow - 4;//a~bまでのrandShakeNowの最大値から半分を引いて負の数も含むように
			pos.y = pos.y + rand() % randShakeNow - 4;
			player_->SetPosition(pos);

			if (pShakeTimer_ <= 0) {
				pDamFlag = false;
			}//0なったらくらい状態解除
		}
		else { pShakeTimer_ = pShakeTime; }
	}
	if (pDamFlag == false) { DebugText::GetInstance()->Print("pdamflag=false", 100, 310, 2); }
	if (pDamFlag == true) { DebugText::GetInstance()->Print("pdamflag=true", 100, 310, 2); }

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
	const float PadCamMoveAmount = 0.7f;

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

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.4f, 0);

						if (NowBossHP <= 0) {
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
				smallenemyForm.radius = se->GetScale().x + 7.f;//余裕を持たせる分＋

				// 当たったら消える
				if (Collision::CheckSphere2Sphere(pBulForm, smallenemyForm)) {
					GameSound::GetInstance()->PlayWave("se_baaan1.wav", 0.1f, 0);
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
						NowPlayerHP -= eBulPower;//自機ダメージ

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
						if (NowPlayerHP <= 0) {//HP0で死亡
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
						pDamFlag = true;
						NowPlayerHP -= seBulPower;//自機ダメージ

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						if (NowPlayerHP <= 0) {//HP0で死亡
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
	const bool PadTriggerA= input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp= input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown= input->TriggerButton(static_cast<int>(Button::DOWN));

	//選択中サイズでっかく
	sp_continuation->SetSize({ PauseSelectSize,100.f });
	sp_continuation->TransferVertexBuffer();

	if (TriggerDown|| PadTriggerDown) {//1を次は選択
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 1;
	}
	if (TriggerUp|| PadTriggerUp) {//上で2
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 2;
	}

	//継続
	if (TriggerEnter|| PadTriggerA)
	{
		PauseFlag = false;
	}

}
void GamePlayScene::PauseOper()
{
	Input* input = Input::GetInstance();
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	//パッド押している間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//選択中サイズでっかく
	sp_operation->SetSize({ PauseSelectSize,100.f });
	sp_operation->TransferVertexBuffer();

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
	/*
	////選択中表示　デバッグ用
	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", PauseNowSelect);
		DebugText::GetInstance()->Print(tmp, 430, 460, 5);
	}*/

	//メンバ関数ポインタ対応した選択
	if(PauseNowSelect == 0){ pFunc = &GamePlayScene::PauseConti; }
	if(PauseNowSelect == 1){ pFunc = &GamePlayScene::PauseOper; }
	if(PauseNowSelect == 2){ pFunc = &GamePlayScene::PauseGoTitle; }
	
	//メンバ関数ポインタ呼び出し
	(this->*pFunc)();

}

void GamePlayScene::Update()
{
	Input* input = Input::GetInstance();
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	const bool TriggerESC = input->TriggerKey(DIK_ESCAPE);

	if (Trigger0) {
		PauseFlag = true;
	}
	if (PauseFlag == true) {
		Pause();
		if (Trigger1) {
			PauseFlag = false;
		}
	}

	//ポーズでないとき～
	//この外はポーズ中も実行
	if (PauseFlag == false)
	{

		Input* input = Input::GetInstance();
		DxBase* dxBase = DxBase::GetInstance();

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

		if (TriggerR) {//デバック用　適当に　いつかは消す
			camera->SetTarget({  });
			camera->SetEye({  });
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

		//くらったらクールタイム
		CoolTime();

		//敵のHPバー
		if (BossEnemyAdvent == true)
		{
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
		//サイズ変更によるズレ--いつか消すから仮
		{
			XMFLOAT3 pHpBar = sp_playerhpbar->GetPosition();
			if (NowPlayerHP <= 700 && BarPosControlOnlyOnceFlag3 == false) {
				pHpBar.x += 30;
				BarPosControlOnlyOnceFlag3 = true;
			}
			if (NowPlayerHP <= 600 && BarPosControlOnlyOnceFlag1 == false) {
				pHpBar.x += 20;
				BarPosControlOnlyOnceFlag1 = true;
			}
			if (NowPlayerHP <= 500 && BarPosControlOnlyOnceFlag4 == false) {
				pHpBar.x += 20;
				BarPosControlOnlyOnceFlag4 = true;
			}
			if (NowPlayerHP <= 400 && BarPosControlOnlyOnceFlag5 == false) {
				pHpBar.x += 10;
				BarPosControlOnlyOnceFlag5 = true;
			}
			if (NowPlayerHP <= 300 && BarPosControlOnlyOnceFlag6 == false) {
				pHpBar.x += 10;
				BarPosControlOnlyOnceFlag6 = true;
			}
			if (NowPlayerHP <= 200 && BarPosControlOnlyOnceFlag7 == false) {
				pHpBar.x += 10;
				BarPosControlOnlyOnceFlag7 = true;
			}

			if (input3) { pHpBar.x += -1.f; }
			sp_playerhpbar->SetPosition(pHpBar);
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

		CollisionAll();
		DrawUI();
		//パッド右スティックカメラ視点
		PadStickCamera();

		// マウス情報の更新
		UpdateMouse();
		// カメラの更新
		camera->Update();
		UpdateCamera();

		//プレイヤー移動
		PlayerMove();

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
			//ボス戦突入のお知らせです
			BossEnemyAdvent = true;
			//残っている雑魚敵はもういらない
			for (auto& se : smallEnemys_) {//いる雑魚敵の分だけ
				se->SetAlive(false);//消す
			}

			for (std::unique_ptr<Boss>& boss : boss_) {
				boss->Update();//敵更新
			}
		}
		// FBX Update
		//fbxObject_1->Update();

		//スプライト更新
		sprite_back->Update();
		sp_guide->Update();
		sp_playerhpbar->Update();
		sp_playerhpbarwaku->Update();
		sp_semeter->Update();
		sp_pause->Update();
		sp_continuation->Update();
		sp_gotitle->Update();
		sp_operation->Update();
		//敵のHPバー
		if (BossEnemyAdvent == true)
		{
			//更新
			sp_enemyhpbar->Update();
			sp_enemyhpbarwaku->Update();
		}

		player_->Update();
	}

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
	//obj_sword->Draw();
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
	sp_semeter->Draw();
	if (PauseFlag == true) {
		sp_pause->Draw();
		sp_continuation->Draw();
		sp_gotitle->Draw();
		sp_operation->Draw();
	}
	if (BossEnemyAdvent == true) { sp_enemyhpbar->Draw(); sp_enemyhpbarwaku->Draw(); }//ボス戦時のみ表示
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
   // sprite->Draw();

}

void GamePlayScene::DrawUI()
{
	//条件なし常に表示
	DebugText::GetInstance()->Print("---PLAYSCENE---", 100, 70, 2);
	DebugText::GetInstance()->Print("[LEFT CLICKorSPACEorPAD ZR] Firing", 100, 100, 2);
	DebugText::GetInstance()->Print("[WASD&QZorGAMEPAD:STICK]PlayerMove", 100, 130, 2);
	DebugText::GetInstance()->Print("[ALLOWorMOVE MOUSEorJ,K,L,I] PlayerRot", 100, 160, 2);
	DebugText::GetInstance()->Print("[ESC] CLOSE WINDOW", 100, 190, 2);
	DebugText::GetInstance()->Print("Player HP", 150, 610, 2);
	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f,%2.f,%2.f", player_->GetPosition().x, player_->GetPosition().y, player_->GetPosition().z);
		DebugText::GetInstance()->Print(tmp, 430, 220, 3);
	}
	//if (NowBossHP == 0) {
	//	DebugText::GetInstance()->Print("crushing!", 100, 230, 3);
	//}
	//if (player_->GetAlive()) {
	//	DebugText::GetInstance()->Print("Alive", 100, 270, 3);
	//}
	//else { DebugText::GetInstance()->Print("GameOver", 100, 270, 3); }

	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {//ボス戦時のみ
		DebugText::GetInstance()->Print("Boss HP", 500, 10, 2);
		DebugText::GetInstance()->Print("!!!Boss!!!", 100, 415, 3);
	}
	else {//ボス戦じゃないときのみ表示
		//雑魚敵撃破数関連
		{
			DebugText::GetInstance()->Print("[BossTerms]", 100, 400, 2);

			char tmp[32]{};
			sprintf_s(tmp, 32, "%2.f", BossTermsEMurdersNum);
			DebugText::GetInstance()->Print(tmp, 300, 390, 3);
		}
		{
			DebugText::GetInstance()->Print("[Now DefeatedEnemy]", 100, 440, 2);

			char tmp[32]{};
			sprintf_s(tmp, 32, "%2.f", sEnemyMurdersNum);
			DebugText::GetInstance()->Print(tmp, 430, 430, 3);
		}
	}
}
