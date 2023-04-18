#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "Timer.h"
#include "DxBase.h"
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

	particle.reset(new ParticleManager());

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	//デバイスをセット
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());

	CharParameters* charParameters = CharParameters::GetInstance();

	//時間
	time = (float)frame / 60.f;	// 60fps想定

	//最初は開始演出
	updatePattern = std::bind(&GamePlayScene::GameReadyUpdate, this);

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
	mod_bossColli.reset(Model::LoadFromOBJ("boss_Colli"));
	//mod_firingline.reset(Model::LoadFromOBJ("firing_line"));
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
	obj_kaberight->SetPosition({ 490,300,2000 });
	obj_kabeleft->SetPosition({ -490,300,2000 });
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
	//最初の演出
	ApEndPPos = { 0,70,-250 };
	ApStartPPos = ApEndPPos;
	ApStartPPos.z -= 1000;
	//
	player_->SetPosition(ApStartPPos);
	camera->SetTarget(player_->GetPosition());
	camera->SetEye({ 0,100,-1000 });//ここにカメラをおいて、最初の演出で自機を追いかける

	boss_.emplace_front();
	for (std::unique_ptr<Boss>& boss : boss_) {//ボス
		boss = std::make_unique<Boss>();
		boss->Initialize();
		boss->SetModel(mod_bossColli.get());
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

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_beforeboss.reset(Sprite::Create(14, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_ready.reset(Sprite::Create(15, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_ready_go.reset(Sprite::Create(16, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sp_ready->isInvisible = false;
	sp_ready_go->isInvisible = true;

	sprite_back->TransferVertexBuffer();

	charParameters->Initialize();

	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });

	// パーティクル初期化
	particle->SetCamera(camera.get());

	csvData = loadCsv("Resources/SmallEnemy.csv", true, ',', "//");

	//時間リセット。タイトルに戻る度。
	Timer* timer = Timer::GetInstance();
	timer->TimerPlay(false);

	//今あるパーティクルを削除する
	particle->DeleteParticles();
}

void GamePlayScene::Finalize()
{
	PostEffect::GetInstance()->SetVignettePow(0.f);
}

void GamePlayScene::GroundMove()
{
	float num = std::sin((float)time * SwingSp) * SwingDist;
	//地面の数だけ
	for (auto& i : obj_ground) {
		XMFLOAT3 pos = i.second->GetPosition();
		pos.y = groundPosDef + num;//初期位置＋揺らす値
		i.second->SetPosition(pos);
		num = -num;//二枚目は逆に揺らす

		i.second->Update();
	}
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

void GamePlayScene::SmallEnemyAimBul()
{
	//雑魚敵の撃つ弾がプレイヤーのいた場所に飛んでいく
	for (auto& se : smallEnemys_) {
		//ターゲット
		se->SetShotTag(player_.get());
	}
}

void GamePlayScene::DoorOpen()
{
	int LDoorPosXRim = -2200;//左の壁開け終わる場所
	int DoorMoveSp = 6;//ドアが開く速度

	XMFLOAT3 LDoorPos = obj_kabeleft->GetPosition();
	XMFLOAT3 RDoorPos = obj_kaberight->GetPosition();

	//左の壁が一定行ったら終わり
	if (LDoorPos.x < LDoorPosXRim) {
		DoorOpenFlag = true;
	}
	else {
		LDoorPos.x -= DoorMoveSp;
		RDoorPos.x += DoorMoveSp;
	}
	obj_kabeleft->SetPosition(LDoorPos);
	obj_kaberight->SetPosition(RDoorPos);
}

void GamePlayScene::BeforeBossAppear()
{
	//演出中時のみtrue
	BeforeBossAppearNow = true;

	const uint32_t BBPaternCountNum = 2;
	const float WarningW = 0.03f;//透明度変化値

	XMFLOAT4 SP_BossWarning = sp_beforeboss->GetColor();

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

void GamePlayScene::BossDeathEffect()
{
	{
		const uint32_t frameMax = 60;//この時間かけて戻す
		if (!(pRotReturnFrame == frameMax)) {
			//カメラ回転と自機の回転同時戻す
			pRotReturnFrame++;
			float raito = (float)pRotReturnFrame / frameMax;
			XMFLOAT3 rot{};
			rot.x = std::lerp(pClearRot.x, 0.f, raito);
			rot.y = std::lerp(pClearRot.y, 0.f, raito);
			rot.z = std::lerp(pClearRot.z, 0.f, raito);
			player_->SetRotation(rot);
		}//回転戻し終わったらもう移動攻撃しない
		else {
			player_->pAtkPossibleFlag = false;
			PDontMoveFlag = true;
			pTracking = false;
			camera->SetTrackingTarget(nullptr);
		}
	}

	if (pClearMoveCount == 0) {
		const uint32_t frameMax = 120;//この時間かけて移動する
		const float pClearMoveEndPos = pBossBattlePos.z + 2000;//ここまで移動
		float raito = (float)clearPMoveFrame / frameMax;
		++clearPMoveFrame;
		XMFLOAT3 pos = player_->GetPosition();
		pos.z = std::lerp(pBossBattlePos.z, pClearMoveEndPos, raito);
		player_->SetPosition(pos);
	}
	else {
		pClearMoveCount--;
	}

	//todo 決めうちなおす
	if (boss_.front()->GetPosition().y < -150.f)
	{
		boss_.front()->SetAlive(false);
	}

	//ボス撃破でクリア　Update内だと一瞬画面見えちゃうからここに
	if (!boss_.front()->GetAlive()) {
		GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGMやめ
		BaseScene* scene = new ClearScene();
		sceneManager_->SetNextScene(scene);
	}
}
void GamePlayScene::BodyDamCoolTime()
{
	if (BodyDamFlag == false) { return; }
	if (--BodyDamCount > 0) { return; }

	BodyDamCount = BodyDamCountDef;
	BodyDamFlag = false;//もう一度喰らう
}

void GamePlayScene::PlayerMove()
{
	ComplexInput* cInput = ComplexInput::GetInstance();

	XMFLOAT3 rotation = player_->GetRotation();

	//自機移動中かどうか false:してない
	bool isLMove = false;
	bool isRMove = false;

	XMFLOAT3 PlayerPos = player_->GetPosition();

	//------------------↓プレイヤー移動＆姿勢
	if (cInput->LeftMove() || cInput->RightMove() || cInput->UpMove() || cInput->DownMove())// inputQ || inputZ ||
	{
		//----------↓移動制限
		const float PlayerMoveLimX = 600;

		const float PlayerMaxMoveLimY = 400;//下に行ける範囲
		const float PlayerMinMoveLimY = 0;//上に行ける範囲

		PlayerPos.x = std::clamp(PlayerPos.x, -PlayerMoveLimX, PlayerMoveLimX);
		PlayerPos.y = std::clamp(PlayerPos.y, PlayerMinMoveLimY, PlayerMaxMoveLimY);

		//----------↑移動制限

		//------プレイヤーも同じ移動------//
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

		PlayerDash();
	}
	else {
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

	//firingline_->SetPosition(PlayerPos);

	player_->SetRotation(rotation);
	//firingline_->SetRotation(rotation);
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
		XMFLOAT3 pPos = player_->GetPosition();
		pPos.x += DashVel.x;
		pPos.y += DashVel.y;
		player_->SetPosition(pPos);
		//firingline_->SetPosition(pPos);

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
		if (--DashInterval == 0) {
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

	pNextPlaceGoSp = std::min(pNextPlaceGoSp, pNextPlaceGoSpMax);//Y座標はPosYMaxまでしかいけないように
	pNextPlaceGoSp += AccelVal;

	XMFLOAT3 pPos = player_->GetPosition();

	//指定した場所超えたら
	if (pPos.z > charParams->StopPos) {
		pNextPlaceGoSp -= DecelVal;

		if ((pNextPlaceGoSp - DecelVal) < 0) {
			charParams->pNextPlaceGoFlag = false;//移動完了でボス行動開始
			pBossBattlePos = pPos;//ボス戦時の自機座標
			//攻撃可能にしてから終わる
			player_->pAtkPossibleFlag = true;
			player_->SetFireLineDrawFlag(true);//射線表示してから雑魚戦
			updatePattern = std::bind(&GamePlayScene::BossBattleUpdate, this);//ボス戦UPDATE
		}
	}

	pPos.z += pNextPlaceGoSp;

	player_->SetPosition(pPos);
}

void GamePlayScene::CoolTime()
{
	constexpr float DamEffectPow = 0.03f;

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

}

void GamePlayScene::ChangeGameOverScene()
{
	GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGMやめ
	BaseScene* scene = new GameOver();
	sceneManager_->SetNextScene(scene);
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

void GamePlayScene::PlayerErase()
{
	GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.3f, 0);
	player_->SetAlive(false);
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
							particle->CreateParticle(boPos, 100, 50, 5);
							bo->BossDamageEffectFlag = true;//くらい演出オン
						}
						Damage = pBulPow - BossDefense;
						NowBoHp -= Damage;
						charParams->SetNowBoHp(NowBoHp);//ボスHPセット

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.4f, 0);

						if (NowBoHp <= 0) {
							GameSound::GetInstance()->PlayWave("bossdeath.wav", 0.3f, 0);
							bo->SetisDeath(true);
							XMFLOAT3 pRot = player_->GetRotation();
							pClearRot = pRot;//ボス撃破時自機どれくらい回転してたか
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
					particle->CreateParticle(sePos, 300, 80, 5, { 1,0.1f,0.8f }, { 1,0,0 });
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
		playerForm.radius = player_->GetScale().z;
		//ボスHPがあるとき
		if (player_->GetAlive() && (NowBoHp > 0)) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& bob : bo->GetBullets()) {
					Sphere eBulForm;
					eBulForm.center = XMLoadFloat3(&bob->GetPosition());
					eBulForm.radius = bob->GetScale().z;

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
		playerForm.radius = player_->GetScale().z;
		//ボスのHPあるとき
		if (player_->GetAlive() && (NowBoHp > 0)) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& boaimbul : bo->GetAimBullets()) {
					Sphere aimBulForm;
					aimBulForm.center = XMLoadFloat3(&boaimbul->GetPosition());
					aimBulForm.radius = boaimbul->GetScale().z;

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
		playerForm.radius = player_->GetScale().z;
		//ボスのHPあるとき
		if (player_->GetAlive() && (NowBoHp > 0)) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& boStraightBul : bo->GetStraightBullets()) {
					Sphere straightBulForm;
					straightBulForm.center = XMLoadFloat3(&boStraightBul->GetPosition());
					straightBulForm.radius = boStraightBul->GetScale().z;

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
		playerForm.radius = player_->GetScale().z;

		if (player_->GetAlive()) {
			for (auto& se : smallEnemys_) {
				if (!se->GetAlive())continue;
				for (auto& seb : se->GetBullets()) {//seb 雑魚敵弾
					Sphere seBulForm;
					seBulForm.center = XMLoadFloat3(&seb->GetPosition());
					seBulForm.radius = seb->GetScale().z;

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
		playerForm.radius = player_->GetScale().z;

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
}

//------------------------------↑当たり判定ZONE↑-----------------------------//

void GamePlayScene::GameReadyUpdate()
{
	CharParameters* charParameters = CharParameters::GetInstance();
	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();

	XMFLOAT4 ReadyCol = sp_ready->GetColor();

	const float ReadyColWDecVal = 0.005f;//Readyを透明にしていく
	const float GoSizeIncVal = 7.f;//Readyを透明にしていく

	constexpr int frameMax = 60;

	if (sceneChangeDirection->OpenTheScreenFlag == false) {//シーン遷移画像残ってるなら
		sceneChangeDirection->GameReadyStartFlag = true;//PlaySceneスタート前になった
		//シーン遷移演出更新
		sceneChangeDirection->Update();
	}
	else {//演出画像開き切ったら
		if (GameReadyFrame < frameMax)
		{
			sp_ready->isInvisible = false;

			//最初演出中は動くな
			PDontMoveFlag = true;

			float raito = (float)GameReadyFrame / frameMax;
			++GameReadyFrame;
			ReadyCol.w = 1.f - raito;
			sp_ready->SetColor({ ReadyCol });
			sp_ready->Update();

			XMFLOAT3 pos{};
			pos.x = std::lerp(ApStartPPos.x, ApEndPPos.x, raito);
			pos.y = std::lerp(ApStartPPos.y, ApEndPPos.y, raito);
			pos.z = std::lerp(ApStartPPos.z, ApEndPPos.z, raito);
			player_->SetPosition(pos);

			if (GameReadyFrame == frameMax) {
				pTracking = true;
			}

			camera->SetTarget(pos);
		}
		else {

			sp_ready->isInvisible = true;

			XMFLOAT4 GOCol = sp_ready_go->GetColor();
			XMFLOAT2 GOSize = sp_ready_go->GetSize();
			XMFLOAT3 GOPos = sp_ready_go->GetPosition();

			const float GoColWDecVal = 0.05f;//GOを透明にしていく
			GOCol.w -= GoColWDecVal;
			GOSize.x += GoSizeIncVal;
			GOSize.y += GoSizeIncVal;
			GOPos.x -= 3.2f;
			GOPos.y -= 3.2f;
			sp_ready_go->isInvisible = false;
			sp_ready_go->SetSize({ GOSize });
			sp_ready_go->TransferVertexBuffer();
			sp_ready_go->SetColor({ GOCol });
			sp_ready_go->SetPosition({ GOPos });
			sp_ready_go->Update();

			if (GOCol.w < 0.f) {//透明になったら
				//アタック開始してよき
				player_->pAtkPossibleFlag = true;
				//動いていいよ
				PDontMoveFlag = false;
				sp_ready_go->isInvisible = true;

				player_->SetFireLineDrawFlag(true);//射線表示してから雑魚戦
				//次は雑魚戦
				updatePattern = std::bind(&GamePlayScene::SmallEnemyBattleUpdate, this);
			}
		}
	}
}

void GamePlayScene::SmallEnemyBattleUpdate()
{
	SmallEnemyAppear();//雑魚的出現関数
	SmallEnemyAimBul();//雑魚敵狙い弾
	UpdateCamera();
	//パッド右スティックカメラ視点移動
	PadStickCamera();

	//雑魚敵更新
	if (BossEnemyAdvent == false) {
		for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
			smallEnemy->Update();
		}
	}

	//撃破数達成
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		player_->SetFireLineDrawFlag(false);//射線表示解除
		//ボス戦前演出
		updatePattern = std::bind(&GamePlayScene::BossBattleReadyUpdate, this);
	}
}

void GamePlayScene::BossBattleReadyUpdate()
{
	UpdateCamera();
	//パッド右スティックカメラ視点移動
	PadStickCamera();

	CharParameters* charParams = CharParameters::GetInstance();

	//雑魚敵更新
	if (BossEnemyAdvent == false) {
		for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
			smallEnemy->Update();
		}
	}
	//残っている雑魚敵はもういらない
	for (auto& se : smallEnemys_) {//いる雑魚敵の分だけ
		se->SetAlive(false);//消す
	}

	//ボス戦前の演出
	if (BeforeBossAppearFlag) {//演出終わったら
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

	sp_beforeboss->Update();//アラート画像
	charParams->boHpUpdate();//Hp画像
}

void GamePlayScene::BossBattleUpdate()
{
	UpdateCamera();
	//パッド右スティックカメラ視点移動
	PadStickCamera();

	CharParameters* charParams = CharParameters::GetInstance();

	//敵のHPバー
	float NowBoHp = charParams->GetNowBoHp();//現在のぼすHP取得
	if (BossEnemyAdvent && NowBoHp > 0) {
		charParams->boHpUpdate();
	}

	//狙い弾どこに打つか
	for (auto& bo : boss_) {
		bo->SetShotTag(player_.get());
	}

	for (std::unique_ptr<Boss>& boss : boss_) {
		boss->Update();//ボス更新

		if (boss->GetisDeath()) {
			player_->SetFireLineDrawFlag(false);
			updatePattern = std::bind(&GamePlayScene::AfterBossBattleUpdate, this);//ボス撃破
		}
	}

	BodyDamCoolTime();//体継続ダメージ
}

void GamePlayScene::AfterBossBattleUpdate()
{
	for (std::unique_ptr<Boss>& boss : boss_) {
		boss->Update();//ボス更新
	}
	BossDeathEffect();//死亡条件達成で死亡時えふぇくと
}

void GamePlayScene::Update()
{
	time = ++frame / 60.f;

	Pause* pause = Pause::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();
	CharParameters* charParams = CharParameters::GetInstance();

	if (pause->GetPauseFlag()) {
		pause->PauseNow();

		if (pause->GetSceneChangeTitleFlag()) {
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			Input::GetInstance()->PadVibration();//振動
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");// 音声停止
			//シーン切り替え
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	UpdateMouse();//ポーズしてるときもマウス更新　元はPause関数内

	if (pTracking) {
		camera->SetTrackingTarget(player_.get());
	}

	//ポーズでないとき～
	//--------------この外に出すとポーズ中も実行
	if (pause->GetPauseFlag() == false) {
		if (pause->WaitKeyP < 10) {
			pause->WaitKeyP++;//ポーズから入力待つ。1フレで開いて閉じちゃうから2回押した的な感じになっちゃう
		}
		else if (pause->WaitKeyP >= 2) {//ある程度経ったら受付
			if (charParams->GetNowpHp() > 0 && charParams->GetNowBoHp() > 0) {//生存時
				if (cInput->PauseOpenClose()) {
					pause->EveryInit();
					GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
					pause->SetPauseFlag(true);
				}
			}
		}

		DxBase* dxBase = DxBase::GetInstance();

		// 自機体力が0より多ければ
		if (player_->GetPHpLessThan0() == false) {
			if (PDontMoveFlag == false) {//自機動くなといわれてないときにplayermove
				//プレイヤー移動-上に書くと移動かくつかない
				PlayerMove();
			}
		}
		if (player_->GetpDeath()) {
			PlayerErase();//自機死亡時消す
		}

		// カメラの更新
		camera->Update();

		//メンバ関数ポインタ呼び出し
		updatePattern();

		GroundMove();//地面揺らす
		obj_groundBottom->Update();
		obj_kaberight->Update();
		obj_kabeleft->Update();
		obj_tunnel->Update();
		obj_backwall->Update();

		//スプライト更新
		sprite_back->Update();
		charParams->pHpUpdate();

		pause->SpUpdate();

		player_->Update();

		//----------------↓シーン切り替え関連↓----------------//
		//自機HP0でゲームオーバー
		if (!player_->GetAlive()) {
			ChangeGameOverScene();
		}
		//----------------↑シーン切り替え関連↑---------------//

		if (player_->GetPHpLessThan0() == false) {
			CollisionAll();
		}

		// パーティクル更新
		particle->Update();

	}//ここまでポーズしてないとき

	//くらったらクールタイム
	CoolTime();

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

	// パーティクル描画
	DxBase* dxBase = DxBase::GetInstance();
	particle->Draw(dxBase->GetCmdList());

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
	if (pause->GetPauseFlag() == false){
		charParameters->pHpDraw();
		pause->SpOpenPauseDraw();
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

	sp_ready->Draw();
	sp_ready_go->Draw();

	SceneChangeDirection* sceneChangeDirection = SceneChangeDirection::GetInstance();
	sceneChangeDirection->Draw();//シーン遷移演出描画

	//向こうでダメージくらい状態解除したらこっちでも同様
	if (charParameters->GetispDam() == false) {
		pDamFlag = false;
	}

	charParameters->DrawUI();

	//{
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "PlayerZ : %2.f", player_->GetPosition().z);
	//	DebugText::GetInstance()->Print(tmp, 150, 220, 1);
	//}
}