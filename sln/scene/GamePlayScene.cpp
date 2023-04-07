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
#include "SceneChangeDirection.h"

#include "TitleScene.h"
#include "ClearScene.h"
#include "GameOver.h"

#include "FbxObject3d.h"
#include "Collision.h"
#include "WinApp.h"

#include "../safe_delete.h"

#include "PostEffect.h"

#include <fstream>
#include <sstream>

#include <DirectXMath.h>
#include <algorithm>

#ifdef min
#undef min
#endif // min

std::vector<std::vector<std::string>> loadCsv(const std::string& csvFilePath,
	bool commentFlag,
	char divChar,
	const std::string& commentStartStr)
{
	std::vector<std::vector<std::string>> csvData{};	// csvの中身を格納

	std::ifstream ifs(csvFilePath);
	if (!ifs)
	{
		return csvData;
	}

	std::string line{};
	// 開いたファイルを一行読み込む(カーソルも動く)
	while (std::getline(ifs, line))
	{
		// コメントが有効かつ行頭が//なら、その行は無視する
		if (commentFlag && line.find(commentStartStr) == 0U)
		{
			continue;
		}

		// 行数を増やす
		csvData.emplace_back();

		std::istringstream stream(line);
		std::string field;
		// 読み込んだ行を','区切りで分割
		while (std::getline(stream, field, divChar))
		{
			csvData.back().emplace_back(field);
		}
	}

	return csvData;
}

void GamePlayScene::Initialize()
{
	camera.reset(new CameraTracking());

	Object3d::SetCamera(camera.get());

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	//デバイスをセット
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());

	CharParameters* charParameters = CharParameters::GetInstance();

	//時間
	time = frame / 60.f;	// 60fps想定

	//------objからモデルデータ読み込み---
	mod_groundBottom.reset(Model::LoadFromOBJ("ground_bottom"));
	mod_kaberight.reset(Model::LoadFromOBJ("Rkabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_smallenemy.reset(Model::LoadFromOBJ("SmallEnemy"));
	mod_playerbullet.reset(Model::LoadFromOBJ("bullet"));
	mod_enemybullet.reset(Model::LoadFromOBJ("enemyBul"));
	mod_bossaimbullet.reset(Model::LoadFromOBJ("BossAimBul"));
	mod_straightbul.reset(Model::LoadFromOBJ("StraightBul"));
	mod_player.reset(Model::LoadFromOBJ("player"));
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
	mod_firingline.reset(Model::LoadFromOBJ("firing_line"));
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	mod_backwall.reset(Model::LoadFromOBJ("back_wall"));

	//------3dオブジェクト生成------//
	// todo 上に書いたほうが手前にあったら描画されない
	obj_groundBottom.reset(Object3d::Create());
	obj_ground.emplace("ground_gre", Object3d::Create());
	obj_ground.emplace("ground_mag", Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_tunnel.reset(Object3d::Create());
	obj_backwall.reset(Object3d::Create());

	for (auto& i : obj_ground) {
		auto& model = mod_ground.emplace(i.first, Model::LoadFromOBJ(i.first)).first;
		model->second->SetTiling({ 10,100 });
		i.second->SetModel(mod_ground.at(i.first).get());
		i.second->SetScale({ 1000.0f, 1.f, 10000.0f });
	}
	obj_ground.at("ground_mag")->SetPosition({ 0,-299,0 });

	//------3dオブジェクトに3dモデルを紐づける------//
	obj_groundBottom->SetModel(mod_groundBottom.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_tunnel->SetModel(mod_tunnel.get());
	obj_backwall->SetModel(mod_backwall.get());
	//------object3dスケール------//
	obj_groundBottom->SetScale({ 10000.0f, 10000.0f, 10000.0f });
	obj_kaberight->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_kabeleft->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_tunnel->SetScale({ 100.0f, 40.0f, 40.0f });
	//------object3d位置------//
	obj_groundBottom->SetPosition({ 0,-220,0 });
	obj_kaberight->SetPosition({ 490,340,2000 });
	obj_kabeleft->SetPosition({ -490,340,2000 });
	obj_tunnel->SetPosition({ 0,40,1000 });
	obj_backwall->SetPosition({ 0,40,7000 });
	//------object回転------//
	obj_kaberight->SetRotation({ 0,0,0 });
	obj_kabeleft->SetRotation({ 0,180,0 });
	obj_tunnel->SetRotation({ 0,-90,0 });

	//いろいろ生成
	player_.reset(new Player());
	//いろいろキャラ初期化
	player_->Initialize();
	player_->SetModel(mod_player.get());
	player_->SetPBulModel(mod_playerbullet.get());
	player_->SetPFiringLine(mod_firingline.get());
	camera->SetTarget(player_->GetPosition());
	camera->SetEye({ 0,100,-1000 });//ここにカメラをおいて、最初の演出で自機を追いかける

	//いろいろ生成
	firingline_.reset(new PlayerFireLine());
	//いろいろキャラ初期化
	firingline_ = std::make_unique<PlayerFireLine>();
	firingline_->Initialize();
	firingline_->SetModel(mod_firingline.get());

	//最初の演出
	ApEndPPos = player_->GetPosition();
	ApStartPPos = ApEndPPos;
	ApStartPPos.z -= 1000;

	boss_.emplace_front();
	for (std::unique_ptr<Boss>& boss : boss_)//ボス
	{
		boss = std::make_unique<Boss>();
		boss->Initialize();
		boss->SetModel(mod_enemy.get());
		boss->SetBulModel(mod_enemybullet.get());
		boss->SetAimBulModel(mod_bossaimbullet.get());//狙い弾
		boss->SetStraightBulModel(mod_straightbul.get());//直線弾
	}

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("E_rhythmaze_128.wav");
	GameSound::GetInstance()->LoadWave("se_baaan1.wav");
	GameSound::GetInstance()->LoadWave("bossdam_1.wav");
	GameSound::GetInstance()->LoadWave("bossdeath.wav");
	GameSound::GetInstance()->LoadWave("playerdeath.wav");
	GameSound::GetInstance()->LoadWave("playerdam.wav");
	GameSound::GetInstance()->LoadWave("dash.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");
	GameSound::GetInstance()->LoadWave("personalgame_bosswarning.wav");
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	Pause* pause = Pause::GetInstance();
	pause->Initialize();
	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	SpriteBase::GetInstance()->LoadTexture(14, L"Resources/Before_Boss.png");
	SpriteBase::GetInstance()->LoadTexture(15, L"Resources/GameReady.png");
	SpriteBase::GetInstance()->LoadTexture(16, L"Resources/GameGO!.png");
	SpriteBase::GetInstance()->LoadTexture(17, L"Resources/BlackWindow.png");

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_beforeboss.reset(Sprite::Create(14, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_ready.reset(Sprite::Create(15, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_ready_go.reset(Sprite::Create(16, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_blackwindow.reset(Sprite::Create(17, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();

	charParameters->Initialize();

	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });

	//スプライトカラー
	sp_blackwindow->SetColor({ 1, 1, 1, 0 });

	// パーティクル初期化
	ParticleManager::GetInstance()->SetCamera(camera.get());

	csvData = loadCsv("Resources/SmallEnemy.csv", true, ',', "//");

	//時間リセット。タイトルに戻る度。
	Timer* timer = Timer::GetInstance();
	timer->TimerPlay(false);

	//今あるパーティクルを削除する
	ParticleManager::GetInstance()->DeleteParticles();
}

void GamePlayScene::Finalize()
{
	PostEffect::GetInstance()->SetVignettePow(0.f);
}

void GamePlayScene::SmallEnemyCreate()
{
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
void GamePlayScene::SmallEnemyAppear()
{
	if (BossEnemyAdvent == false)
	{
		//時が満ちたら
		if (SEneAppCount == 0) {
			//雑魚敵来る
			//csvの最後まで行った場合最初に戻す
			if (++seIndex >= csvData.size()) {
				seIndex = 0;
			}
			SmallEnemyCreate();
			float posx = std::stof(csvData[seIndex][0]);//雑魚敵X座標はcsvの0番目
			float posy = std::stof(csvData[seIndex][1]);
			float posz = std::stof(csvData[seIndex][2]);
			//雑魚敵をcsv通りの場所に出す
			smallEnemys_.back()->SetPosition(XMFLOAT3{ posx,posy,posz });

			//再びカウントできるように初期化
			SEneAppCount = SEneAppInterval;
		}
	}
	//雑魚敵カウントをデクリメント
	SEneAppCount--;
}

void GamePlayScene::BossConditionComp()
{
	//撃破数達成でボス戦
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		CharParameters* charParams = CharParameters::GetInstance();

		//残っている雑魚敵はもういらない
		for (auto& se : smallEnemys_) {//いる雑魚敵の分だけ
			se->SetAlive(false);//消す
		}

		//ボス戦前の演出
		if (BeforeBossAppearFlag)
		{//演出終わったら
			//ボス戦突入のお知らせです
			BossEnemyAdvent = true;
		}
		else {
			BeforeBossAppear();
		}
		//条件達成でボス登場演出
		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Update();//ボス更新

			if (boss->GetisDeath()) {
				BossDeathEffect();//死亡条件達成で死亡時えふぇくと
			}
		}
		//扉を開ける
		if (DoorOpenFlag == false) { DoorOpen(); }

		if (charParams->pNextPlaceGoFlag) {
			pHeadingToTheNextPlace();
		}
	}
}

void GamePlayScene::DoorOpen()
{
	int LDoorPosXRim = -2200;//左の壁開け終わる場所
	int DoorMoveSp = 6;//ドアが開く速度

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

void GamePlayScene::BeforeBossAppear()
{
	//演出中時のみtrue
	BeforeBossAppearNow = true;

	const int BBPaternCountNum = 4;
	const float WarningW = 0.03f;//透明度変化値

	XMFLOAT4 SP_BossWarning = sp_beforeboss->GetColor();
	//SP_BossWarning.w -= 0.01;

	switch (beforeBossPattern_)
	{
	case BeforeBossPattern::def:
		if (AlertSoundFlag) {
			GameSound::GetInstance()->PlayWave("personalgame_bosswarning.wav", 0.1f, 0);
			AlertSoundFlag = false;
		}
		SP_BossWarning.w -= WarningW;
		if (SP_BossWarning.w < 0.0f) {
			beforeBossPattern_ = BeforeBossPattern::inc;
		}
		break;
	case BeforeBossPattern::inc:
		SP_BossWarning.w += WarningW;
		if (SP_BossWarning.w > 1.0f) {
			beforeBossPattern_ = BeforeBossPattern::dec;
			AlertSoundFlag = true;
			BBPaternCount++;//繰り返す回数
		}
		break;

	case BeforeBossPattern::dec:
		if (AlertSoundFlag) {
			GameSound::GetInstance()->PlayWave("personalgame_bosswarning.wav", 0.3f, 0);
			AlertSoundFlag = false;
		}
		SP_BossWarning.w -= WarningW;
		if (SP_BossWarning.w < 0.0) {
			beforeBossPattern_ = BeforeBossPattern::inc;
		}
		break;
	}

	//--繰り返す回数0~------消えてからボス戦へ
	if (BBPaternCount == BBPaternCountNum && beforeBossPattern_ == BeforeBossPattern::inc)
	{
		BeforeBossAppearFlag = true;
		BeforeBossAppearNow = true;
	}

	sp_beforeboss->SetColor(SP_BossWarning);
}

void GamePlayScene::BossBodyRed()
{
	BossBodyRedTime--;//この時間赤くする

	for (auto& bo : boss_) {//赤くする
		XMFLOAT4 col = bo->GetColor();

		col = { 1,ReCol,ReCol,1 };//赤
		ReCol += ReColVal;

		if (BossBodyRedTime == 0) {//時間になったら
			ReCol = 0.f;//戻す
			col = { 1,1,1,1 };//本来の色
			BossBodyRedTime = BossBodyRedTimeDef;//カウント戻す
			BossDamFlag = false;//くらっていない状態に
		}
		bo->SetColor(col);
	}
}

void GamePlayScene::BossDeathEffect()
{
	XMFLOAT4 color = sp_blackwindow->GetColor();
	color.w += colordec;
	sp_blackwindow->SetColor(color);

	//todo 決めうちなおす
	if (boss_.front()->GetPosition().y < -150.f)
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
	ComplexInput* cInput = ComplexInput::GetInstance();

	XMFLOAT3 rotation = player_->GetRotation();

	//------------------↓プレイヤー移動＆姿勢
	if (cInput->LeftMove() || cInput->RightMove() || cInput->UpMove() || cInput->DownMove())// inputQ || inputZ ||
	{
		//----------↓移動制限
		const float PlayerMoveLimX = 600;

		const float PlayerMaxMoveLimY = 400;//下に行ける範囲
		const float PlayerMinMoveLimY = 0;//上に行ける範囲

		//const float PlayerMaxMoveLimZ = 290;//後ろ
		//const float PlayerMinMoveLimZ = 200;

		XMFLOAT3 PlayerPos = player_->GetPosition();
		PlayerPos.x = std::clamp(PlayerPos.x, -PlayerMoveLimX, PlayerMoveLimX);
		PlayerPos.y = std::clamp(PlayerPos.y, PlayerMinMoveLimY, PlayerMaxMoveLimY);

		//----------↑移動制限

		//------プレイヤーも同じ移動------//
		//bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 5.f;

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
		player_->SetPosition(PlayerPos);
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

	player_->SetRotation(rotation);
}

void GamePlayScene::PlayerDash()
{
	//shiftキー、右クリックでダッシュ
	//加減速計算で常に足す値出す⇨実際に足す
	//クールタイム
	ComplexInput* cInput = ComplexInput::GetInstance();

	//キー押されたら
	if (cInput->PlayerDash() && DashFlag == false && DashIntervalFlag == false) {
		//移動中なら
		if (cInput->DownMove() || cInput->UpMove() || cInput->RightMove() || cInput->LeftMove()) {
			//ダッシュをするとき風切り音
			GameSound::GetInstance()->PlayWave("dash.wav", 0.7f, 0);
			DashFlag = true;
			DashIntervalFlag = true;
		}
	}
	//ダッシュスタート
	if (DashFlag) {
		XMFLOAT3 pPos = player_->GetPosition();

		//ダッシュする時間
		DashCount--;

		//まだ方向決める前なら
		//ダッシュ中に反対方向押してもそっち方向にダッシュできないようにする目的で一回決めた方向にしかダッシュできないように
		if (playerDashDirection_ == PlayerDashDirection::def) {
			if (cInput->DownMove()) {
				//下に加速に決めた！
				playerDashDirection_ = PlayerDashDirection::down;
			}
			if (cInput->UpMove()) {
				//上に加速に決めた！
				playerDashDirection_ = PlayerDashDirection::up;
			}
			if (cInput->RightMove()) {
				//右に加速に決めた！
				playerDashDirection_ = PlayerDashDirection::right;
			}
			if (cInput->LeftMove()) {
				//左に加速に決めた！
				playerDashDirection_ = PlayerDashDirection::left;
			}
		}

		//------決めた方向にダッシュ
		if (playerDashDirection_ == PlayerDashDirection::down) {
			DashVel.y = -DashVelInc;
		}
		if (playerDashDirection_ == PlayerDashDirection::up) {
			DashVel.y = DashVelInc;
		}
		if (playerDashDirection_ == PlayerDashDirection::right) {
			DashVel.x = DashVelInc;
		}
		if (playerDashDirection_ == PlayerDashDirection::left) {
			DashVel.x = -DashVelInc;
		}
		//現ダッシュ時間が減衰開始時間になったら
		if (DashCount == (DashCountDef - DashAttenuation)) {
			DashAttenuationFlag = true;//減衰開始
		}

		//移動
		pPos.x += DashVel.x;
		pPos.y += DashVel.y;
		//pPos.z += DashVel.z;

		player_->SetPosition(pPos);

		if (DashCount == 0) {
			playerDashDirection_ = PlayerDashDirection::def;//決定する前に戻す
			DashVelInc = DashVelIncDef;
			DashVel = { 0,0,0 };
			DashCount = DashCountDef;
			DashAttenuationFlag = false;
			DashFlag = false;
		}
	}

	//インターバル計測なう
	if (DashIntervalFlag) {
		DashInterval--;
		if (DashInterval == 0) {
			//ダッシュしてよし
			DashInterval = DashIntervalDef;
			DashIntervalFlag = false;
		}
	}

	if (DashAttenuationFlag) {
		DashVelInc += Attenuation;
	}
}

void GamePlayScene::pHeadingToTheNextPlace()
{
	CharParameters* charParams = CharParameters::GetInstance();

	//攻撃できないように
	player_->pAtkPossibleFlag = false;

	XMFLOAT3 pPos = player_->GetPosition();

	bool StopFlag = false;//停止してね

	pNextPlaceGoSp = std::min(pNextPlaceGoSp, pNextPlaceGoSpMax);//Y座標はPosYMaxまでしかいけないように
	pNextPlaceGoSp += AccelVal;

	if (pPos.z > charParams->StopPos) {//指定した場所超えたら
		StopFlag = true;//止まれ
	}

	if (StopFlag) {
		pNextPlaceGoSp -= DecelVal;

		if ((pNextPlaceGoSp - DecelVal) < 0) {
			charParams->pNextPlaceGoFlag = false;
			//攻撃可能にしてから終わる
			player_->pAtkPossibleFlag = true;
		}
	}

	pPos.z += pNextPlaceGoSp;

	player_->SetPosition(pPos);
}

void GamePlayScene::CoolTime()
{
	//Input* input = Input::GetInstance();
	const float DamEffectPow = 0.03f;

	//くーーーーるたいむ仮　今は文字だけ
	if (pDamFlag) {
		//画像薄くしてく
		vignettePow -= DamEffectPow;
		if (vignettePow < 0.f) {
			//繰り返さないように
			DamEfRedFlag = true;
			vignettePow = 0.f;
		}
		PostEffect::GetInstance()->SetVignettePow(vignettePow);
	}
	else {
		//ダメージ終わったら赤のダメージ画像色戻す
		DamEfRedFlag = false;
		vignettePow = 1.f;
	}
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
	// 自機の視線ベクトル
	{
		//感度
		const float camMoveVel = 0.05f;

		XMFLOAT3 rota = player_->GetRotation();

		//カメラ上下移動制限
		const float rotXrim = 60.f;
		const float rotYrim = 90.f;
		if (rota.x > rotXrim) {
			rota.x = rotXrim;
		}
		if (rota.x < -rotXrim) {
			rota.x = -rotXrim;
		}
		//カメラ左右移動制限
		if (rota.y > rotYrim) {
			rota.y = rotYrim;
		}
		if (rota.y < -rotYrim) {
			rota.y = -rotYrim;
		}
		// マウスの横方向(X)の移動がカメラの縦方向(Y)の回転になる
		rota.x += cameraMoveVel.y * camMoveVel;
		// マウスの縦方向(Y)の移動がカメラの横方向(X)の回転になる
		rota.y += cameraMoveVel.x * camMoveVel;

		player_->SetRotation(rota);
	}

	{//自機射線
		XMFLOAT3 PlayerPos = player_->GetPosition();
		firingline_->SetPosition({ PlayerPos.x,PlayerPos.y,PlayerPos.z });

		XMFLOAT3 PlayerRot = player_->GetRotation();
		firingline_->SetRotation(PlayerRot);
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
	CharParameters* charParams = CharParameters::GetInstance();

	float NowBoHp = charParams->GetNowBoHp();//現在のぼすHP取得
	float BossDefense = charParams->GetBossDefense();//ボス防御力取得 先頭要素
	float NowpHp = charParams->GetNowpHp();//自機体力取得
	float pBulPow = player_->GetpBulPow();//自機弾威力

	//------------------------------↓当たり判定ZONE↓-----------------------------//
	//[自機の弾]と[ボス]の当たり判定                  移動終わったら					自機の体力あるとき
	if (sEnemyMurdersNum >= BossTermsEMurdersNum && charParams->pNextPlaceGoFlag == false && (NowpHp > 0)) {
		{
			Sphere pBulForm;//球

			for (auto& pb : player_->GetBullets()) {
				if (!pb->GetAlive())continue;//死んでたらスキップ
				pBulForm.center = XMLoadFloat3(&pb->GetPosition());
				pBulForm.radius = pb->GetScale().x;

				// 衝突判定をする
				for (auto& bo : boss_) {
					if (!bo->GetAlive())continue;
					if (!bo->GetDoCollision())continue;//ボス側で判定取らないでって言われてたらスキップ
					Sphere enemyForm;
					enemyForm.center = XMLoadFloat3(&bo->GetPosition());
					enemyForm.radius = bo->GetScale().x;

					if (bo->GetisDeath())continue;

					// 当たったら消える
					if (Collision::CheckSphere2Sphere(pBulForm, enemyForm)) {
						XMFLOAT3 boPos = bo->GetPosition();

						pb->SetAlive(false);

						//喰らってまだ生きてたら
						if ((NowBoHp - (pBulPow - BossDefense)) > 0) {
							ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
							BossDamFlag = true;//赤くする
						}
						Damage = pBulPow - BossDefense;
						NowBoHp -= Damage;
						charParams->SetNowBoHp(NowBoHp);//ボスHPセット

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
				smallenemyForm.radius = se->GetScale().x + 20;//余裕を持たせる分＋

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

	//[自機]と[ボス弾]の当たり判定
	{
		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;
		//ボスHPがあるとき
		if (player_->GetAlive() && (NowBoHp > 0)) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& bob : bo->GetBullets()) {
					Sphere eBulForm;
					eBulForm.center = XMLoadFloat3(&bob->GetPosition());
					eBulForm.radius = bob->GetScale().z + 2.f;

					if (Collision::CheckSphere2Sphere(playerForm, eBulForm)) {
						pDamFlag = true;
						NowpHp -= bo->GetBulPow();//自機ダメージ
						charParams->SetispDam(true);
						charParams->SetNowpHp(NowpHp);//プレイヤーHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
						break;
					}
				}
			}
		}
	}
	//[自機]と[ボス狙い弾]の当たり判定
	{
		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;
		//ボスのHPあるとき
		if (player_->GetAlive() && (NowBoHp > 0)) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& boaimbul : bo->GetAimBullets()) {
					Sphere aimBulForm;
					aimBulForm.center = XMLoadFloat3(&boaimbul->GetPosition());
					aimBulForm.radius = boaimbul->GetScale().z + 2.f;

					if (Collision::CheckSphere2Sphere(playerForm, aimBulForm)) {
						pDamFlag = true;
						NowpHp -= bo->GetAimBulPow();//自機ダメージ
						charParams->SetispDam(true);
						charParams->SetNowpHp(NowpHp);//プレイヤーHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						boaimbul->SetAlive(false);
						break;
					}
				}
			}
		}
	}

	//[自機]と[ボス直線弾]の当たり判定
	{
		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;
		//ボスのHPあるとき
		if (player_->GetAlive() && (NowBoHp > 0)) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& boStraightBul : bo->GetStraightBullets()) {
					Sphere straightBulForm;
					straightBulForm.center = XMLoadFloat3(&boStraightBul->GetPosition());
					straightBulForm.radius = boStraightBul->GetScale().z + 2.f;

					if (Collision::CheckSphere2Sphere(playerForm, straightBulForm)) {
						pDamFlag = true;
						NowpHp -= bo->GetStraightBulPow();//自機ダメージ
						charParams->SetispDam(true);
						charParams->SetNowpHp(NowpHp);//プレイヤーHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						boStraightBul->SetAlive(false);
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
						charParams->SetispDam(true);//自機くらい
						charParams->SetNowpHp(NowpHp);//ボスHPセット

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						break;
					}
				}
			}
		}
	}

	//[ボス]と[自機]の当たり判定
	{
		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;

		if (player_->GetAlive()) {
			// 衝突判定をする
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				Sphere bossForm;
				bossForm.center = XMLoadFloat3(&bo->GetPosition());
				bossForm.radius = bo->GetScale().x;

				if (bo->GetisDeath())continue;

				//定期的にダメージ
				if (BodyDamFlag == false) {
					if (Collision::CheckSphere2Sphere(playerForm, bossForm)) {
						pDamFlag = true;
						int bodyPow = bo->GetBodyPow();//ボス体威力
						NowpHp -= bodyPow;//自機にダメージ
						charParams->SetispDam(true);//自機くらい
						charParams->SetNowpHp(NowpHp);

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						BodyDamFlag = true;//クールたいむ
						break;
					}
				}
			}
		}
	}
	//与えるダメージが0以下だったら1にする
	if (Damage <= 0) {
		Damage = 1;
	}
}

//------------------------------↑当たり判定ZONE↑-----------------------------//

bool GamePlayScene::GameReady()
{
	CharParameters* charParameters = CharParameters::GetInstance();
	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();

	XMFLOAT4 ReadyCol = sp_ready->GetColor();
	XMFLOAT4 GOCol = sp_ready_go->GetColor();
	XMFLOAT2 GOSize = sp_ready_go->GetSize();
	XMFLOAT3 GOPos = sp_ready_go->GetPosition();

	const float ReadyColWDecVal = 0.005f;//Readyを透明にしていく
	const float GoColWDecVal = 0.01f;//GOを透明にしていく
	const float GoSizeIncVal = 7.f;//Readyを透明にしていく

	constexpr int frameMax = 300;

	if (sceneChangeDirection->OpenTheScreenFlag == false) {//シーン遷移画像残ってるなら
		sceneChangeDirection->GameReadyStartFlag = true;//PlaySceneスタート前になった
		//シーン遷移演出更新
		sceneChangeDirection->Update();
	}
	else {//演出画像開き切ったら
		if (GameReadyFrame < frameMax)
		{
			//最初演出中は動くな
			PDontMoveFlag = true;

			float raito = (float)GameReadyFrame / frameMax;
			GameReadyFrame++;
			ReadyCol.w = 1.f - raito;
			sp_ready->SetColor({ ReadyCol });
			sp_ready->Update();

			XMFLOAT3 pos{};
			pos.x = std::lerp(ApStartPPos.x, ApEndPPos.x, raito);
			pos.y = std::lerp(ApStartPPos.y, ApEndPPos.y, raito);
			pos.z = std::lerp(ApStartPPos.z, ApEndPPos.z, raito);
			player_->SetPosition(pos);

			camera->SetTarget(pos);
		}
		else {
			ready_GOFlag = true;
			GOCol.w -= GoColWDecVal;
			GOSize.x += GoSizeIncVal;
			GOSize.y += GoSizeIncVal;
			GOPos.x -= 3.2f;
			GOPos.y -= 3.2f;
			sp_ready_go->SetSize({ GOSize });
			sp_ready_go->TransferVertexBuffer();
			sp_ready_go->SetColor({ GOCol });
			sp_ready_go->SetPosition({ GOPos });
			sp_ready_go->Update();

			if (SetTagOnceFlag == false) {
				camera->SetTrackingTarget(player_.get());
				SetTagOnceFlag = true;
			}
		}
	}

	if (GOCol.w < 0.0) {//透明になったら
		if (MayDoPAtk_OnceFlag == false) {
			//アタック開始してよき
			player_->pAtkPossibleFlag = true;
			//動いていいよ
			PDontMoveFlag = false;

			MayDoPAtk_OnceFlag = true;
		}

		return false;
	}

	return true;
}

void GamePlayScene::BodyDamCoolTime()
{
	if (BodyDamFlag) {
		BodyDamCount--;
		if (BodyDamCount == 0) {
			BodyDamCount = BodyDamCountDef;
			BodyDamFlag = false;//もう一度喰らう
		}
	}
}

void GamePlayScene::Update()
{
	frame += 1.f;
	time = frame / 60.f;

	Pause* pause = Pause::GetInstance();

	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();
	//const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	//パッドトリガー
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));

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

	CharParameters* charParams = CharParameters::GetInstance();

	if (pause->WaitKeyP < 10 && pause->GetPauseFlag() == false) {
		pause->WaitKeyP++;//ポーズから入力待つ。1フレで開いて閉じちゃうから2回押した的な感じになっちゃう
	}
	if (pause->WaitKeyP >= 2) {//ある程度経ったら受付
		if (charParams->GetNowpHp() > 0 && charParams->GetNowBoHp() > 0) {//生存時
			if (cInput->PauseOpenClose() && (GameReady() == false) && pause->GetPauseFlag() == false) {
				pause->EveryInit();
				GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
				pause->SetPauseFlag(true);
			}
		}
	}
	if (pause->GetPauseFlag()) {
		pause->PauseNow();
		UpdateMouse();//ポーズしてるときもマウス更新　元はPause関数内

		if (pause->GetSceneChangeTitleFlag()) {
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();//振動
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");// 音声停止
			//シーン切り替え
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	//ポーズでないとき～
	//--------------この外に出すとポーズ中も実行
	if (pause->GetPauseFlag() == false)
	{
		Input* input = Input::GetInstance();
		DxBase* dxBase = DxBase::GetInstance();

		CharParameters* charParameters = CharParameters::GetInstance();
		float NowBoHp = charParameters->GetNowBoHp();//現在のぼすHP取得
		float BossDefense = charParameters->GetBossDefense();//ボス防御力取得
		float NowpHp = charParameters->GetNowpHp();//自機体力取得

		float pMaxHp = charParameters->GetpMaxHp();
		float boMaxHp = charParameters->GetboMaxHp();

		//自機のHPバー
		charParameters->pHpSizeChange();

		charParameters->BarGetDislodged();

		if (player_->GetPHpLessThan0() == false)
		{
			if (PDontMoveFlag == false) {//自機動くなといわれてないときにplayermove
				//プレイヤー移動-上に書くと移動かくつかない
				PlayerMove();
				PlayerDash();
			}
		}

		DrawUI();
		//パッド右スティックカメラ視点
		PadStickCamera();

		// マウス情報の更新
		UpdateMouse();
		// カメラの更新
		camera->Update();
		if (GameReady() == false) {//ゲーム開始前演出終了後に自機の回転＝カメラ回転できるようになる
			UpdateCamera();
		}

		if (pRotDef == false) { //一度だけ
			input->PadVibrationDef();
			player_->SetRotation({});
			pRotDef = true;
		}

		//3dobjUPDATE
		{
			float num = std::sin((float)time * SwingSp) * SwingDist;
			//地面の数だけ
			for (auto& i : obj_ground) {
				XMFLOAT3 pos = i.second->GetPosition();
				pos.y = PosDef + num;//初期位置＋揺らす値
				i.second->SetPosition(pos);
				num = -num;//二枚目は逆に揺らす

				i.second->Update();
			}
		}
		obj_groundBottom->Update();
		obj_kaberight->Update();
		obj_kabeleft->Update();

		//スプライト更新
		sprite_back->Update();
		charParameters->pHpUpdate();

		pause->SpUpdate();

		player_->Update();
		if (player_->pAtkPossibleFlag) {//攻撃可能時のみ
			firingline_->Update();//射線
		}

		if (GameReady() == false)
		{
			PlayTimer();

			//敵のHPバー
			if (BossEnemyAdvent)
			{
				charParameters->boHpSizeChange();
			}

			SmallEnemyAppear();//雑魚的出現関数

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
			if (player_->GetpDeath()) {
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

			BodyDamCoolTime();//体継続ダメージ
			if (BossDamFlag == true) {
				BossBodyRed();//ダメージ中赤く
			}

			if (player_->GetPHpLessThan0() == false)
			{
				CollisionAll();
			}

			if (input->TriggerKey(DIK_O)) {
				auto pos = camera->GetEye();
				pos.z += 100;
				ParticleManager::GetInstance()->CreateParticle(pos, 300, 80, 5);
			}
			// パーティクル更新
			ParticleManager::GetInstance()->Update();

			//雑魚敵更新
			if (BossEnemyAdvent == false)
			{
				for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
					smallEnemy->Update();
				}
			}
			//ボス戦条件達成でボス戦行くための準備
			BossConditionComp();

			// FBX Update
			//fbxObject_1->Update();

			sp_beforeboss->Update();
			//敵のHPバー
			if (BossEnemyAdvent && NowBoHp > 0)
			{
				charParameters->boHpUpdate();
			}
		}
	}//ここまでポーズしてないとき

	//くらったらクールタイム
	CoolTime();

	obj_tunnel->Update();
	obj_backwall->Update();
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();

	CharParameters* charParameters = CharParameters::GetInstance();
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//// スプライト描画
	sprite_back->Draw();

	//3dオブジェ描画前処理
	Object3d::PreDraw(DxBase::GetInstance()->GetCmdList());

	//雑魚敵
	for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
		smallEnemy->Draw();
	}

	//3dオブジェ描画
	for (auto& i : obj_ground) {
		i.second->Draw();
	}
	obj_groundBottom->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();
	obj_tunnel->Draw();
	obj_backwall->Draw();

	//敵描画
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Draw();
		}
	}

	//自キャラ描画
	player_->Draw();
	if (player_->pAtkPossibleFlag) {//攻撃可能時のみ
		firingline_->Draw();
	}

	// パーティクル描画
	DxBase* dxBase = DxBase::GetInstance();
	ParticleManager::GetInstance()->Draw(dxBase->GetCmdList());

	float NowBoHp = charParameters->GetNowBoHp();//現在のぼすHP取得

	//3dオブジェ描画後処理
	Object3d::PostDraw();
}

void GamePlayScene::DrawUI()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();

	CharParameters* charParameters = CharParameters::GetInstance();

	//---------------お手前スプライト描画
	Pause* pause = Pause::GetInstance();
	if (pause->GetPauseFlag() == false)
	{
		//sp_guide->Draw();
		charParameters->pHpDraw();
		pause->SpOpenPauseDraw();
		//sp_sight->Draw();
	}
	if (pause->GetPauseFlag()) {
		pause->SpFlagTrueNowDraw();
	}
	else if (BossEnemyAdvent && charParameters->GetNowBoHp() > 0) {
		charParameters->boHpDraw();
	}//ボス戦時のみ表示

	if (pause->GetOpWindOpenFlag()) { pause->SpOperWindDraw(); }

	//ボス戦前 ポーズ中は見せない
	if (BeforeBossAppearNow && pause->GetPauseFlag() == false)
	{
		sp_beforeboss->Draw();
	}

	//開始前中のみ
	if (GameReady())
	{
		sp_ready->Draw();
		if (ready_GOFlag) { sp_ready_go->Draw(); };
	}

	for (auto& bo : boss_) {
		if (bo->GetisDeath()) {
			sp_blackwindow->Draw();
		}
	}

	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();
	sceneChangeDirection->Draw();//シーン遷移演出描画

	//向こうでダメージくらい状態解除したらこっちでも同様
	if (charParameters->GetispDam() == false) {
		pDamFlag = false;
	}

	charParameters->DrawUI();
}

void GamePlayScene::PlayTimer()
{
}