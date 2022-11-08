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

	// �}�E�X�J�[�\����\��
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());
	// �J�����Z�b�g
	//Object3d::SetCamera(camera.get());

	//�O���t�B�b�N�X�p�C�v���C������
	//FbxObject3d::CreateGraphicsPipeline();
	//FbxObject3d::SetCamera(camera.get());

	//�g����`�Ƃ��@�������Ƃ���
	time = frame / 60.f;	// 60fps�z��

	//------obj���烂�f���f�[�^�ǂݍ���---
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
	//------3d�I�u�W�F�N�g����------//
	object3d_1.reset(Object3d::Create());
	obj_worlddome.reset(Object3d::Create());
	obj_sword.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	//------3d�I�u�W�F�N�g��3d���f����R�Â���------//
	object3d_1->SetModel(model_1.get());
	obj_worlddome->SetModel(mod_worlddome.get());
	obj_sword->SetModel(mod_sword.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	//------object3d�X�P�[��------//
	object3d_1->SetScale({ 80.0f, 20.0f, 500.0f });
	obj_worlddome->SetScale({ 10.0f, 10.0f, 10.0f });
	obj_sword->SetScale({ 7.0f, 7.0f, 7.0f });
	obj_kaberight->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_kabeleft->SetScale({ 200.0f, 200.0f, 10.0f });
	//------object3d�ʒu------//
	object3d_1->SetPosition({ 0,-150,0 });
	obj_worlddome->SetPosition({ 0,200,150 });
	obj_sword->SetPosition({ 0,50,0 });
	obj_kaberight->SetPosition({ 310,-200,0 });
	obj_kabeleft->SetPosition({ -310,-200,0 });
	//------object��]------//
	obj_kaberight->SetRotation({ 0,90,0 });
	obj_kabeleft->SetRotation({ 0,-90,0 });

	//���낢�됶��
	player_.reset(new Player());
	//���낢��L����������
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
	//----------FBX �I�u�W�F�N�g�����ƃ��f���̃Z�b�g-----------//

	//fbxObject_1 = new FbxObject3d;

	//fbxObject_1->Initialize();

	//------fbx�Z�b�g------//
	//fbxObject_1->SetModel(fbxModel_1);
	//------fbx�X�P�[��------//
	//fbxObject_1->SetScale({ 10.0f, 10.0f, 10.0f });
	//------fbx�ʒu------//
	//fbxObject_1->SetPosition({ 0,24,100 });
	//fbxObject_1->PlayAnimation();

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("E_rhythmaze_128.wav");
	GameSound::GetInstance()->LoadWave("se_baaan1.wav");
	GameSound::GetInstance()->LoadWave("bossdam_1.wav");
	GameSound::GetInstance()->LoadWave("bossdeath.wav");
	GameSound::GetInstance()->LoadWave("playerdeath.wav");
	GameSound::GetInstance()->LoadWave("playerdam.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");
	GameSound::GetInstance()->LoadWave("personalgame_bosswarning.wav");
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav", 0.2f, XAUDIO2_LOOP_INFINITE);
	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];
	CharParameters* charParameters = CharParameters::GetInstance();
	// -----------------�X�v���C�g���ʃe�N�X�`���ǂݍ���
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

	// �X�v���C�g�̐���
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

	//window�c���擾�������Ƃ��g��
	WinApp* winApp = WinApp::GetInstance();
	//�X�v���C�g�|�W�V����
	sprite_back->SetPosition({ -11400,0,0 });
	//sp_guide->SetPosition({ 0,0,0 });
	//sp_sight->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2,0 });
	sp_sight->SetPosition({ 0,0,0 });

	float gotitlePosY = winApp->window_width / 2;
	sp_continuation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 450,0 });//��
	sp_operation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 300,0 });//�^��
	sp_gotitle->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 150 ,0 });//��
	sp_openpause->SetPosition({ 1050,600,0 });
	//---------�X�v���C�g�T�C�Y---------//
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

	// �p�[�e�B�N��������
	ParticleManager::GetInstance()->SetCamera(camera.get());

	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // �X�v���C�g�̍��W�ύX
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);
	//}

	//fbx ���f�����w�肵�ăt�@�C���ǂݍ���
	//FbxLoader::GetInstance()->LoadModelFromFile(
	//	"cube"
	//);

	//int counter = 0; // �A�j���[�V�����̌o�ߎ��ԃJ�E���^�[

	//// �X�v���C���Ȑ�
	////posints = { start, start, p2, p3, end, end }
	//points.emplace_back(XMVectorSet(0, 0, 0, 0));//s
	//points.emplace_back(XMVectorSet(0, 0, 0, 0));//s
	//points.emplace_back(XMVectorSet(0, 0, 60, 0));
	//points.emplace_back(XMVectorSet(0, 70, 100, 0));
	//points.emplace_back(XMVectorSet(0, 30, 50, 0));//e
	//points.emplace_back(XMVectorSet(0, 0, 0, 0));//e
	////p1����X�^�[�g
	//splineStartIndex = 1;

	//���ԃ��Z�b�g�B�^�C�g���ɖ߂�x�B
	Timer* timer = Timer::GetInstance();
	timer->TimerPlay(false);

	//�p�����[�^�֘A������������
	charParameters->SetNowBoHp({ charParameters->GetboMaxHp()});//�{�X�̗�
	charParameters->SetNowpHp({ charParameters->GetpMaxHp()});//���@�̗�
}

void GamePlayScene::Finalize()
{
	//safe_delete(fbxObject_1);
	//safe_delete(fbxModel_1);

	//���L�������
	//delete player_;
	//delete smallEnemy_;
}

void GamePlayScene::SmallEnemyAppear()
{
	//�L�[���͎g��
	//Input* input = Input::GetInstance();

	//�G���G����
	std::unique_ptr<SmallEnemy> madeSmallEnemy = std::make_unique<SmallEnemy>();

	madeSmallEnemy->Initialize();

	madeSmallEnemy->SetModel(mod_smallenemy.get());
	for (std::unique_ptr<SmallEnemy>& se : smallEnemys_)
	{
		se->SetSEBulModel(mod_enemybullet.get());
	}
	//�G���G�o�^
	smallEnemys_.push_back(std::move(madeSmallEnemy));
}

void GamePlayScene::BeforeBossAppear()
{
	//���o�����̂�true
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
			BBPaternCount++;//�J��Ԃ���
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

	//--�J��Ԃ���0~------�����Ă���{�X���
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
	//�p�b�h�����Ă����
	//const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	//const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	//const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	//const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	//----------���ړ�����
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 0;//���ɍs����͈�
	const float PlayerMinMoveLimY = 200;//��ɍs����͈�

	const float PlayerMaxMoveLimZ = 290;//���
	const float PlayerMinMoveLimZ = 200;

	XMFLOAT3 PlayerPos = player_->GetPosition();
	XMFLOAT3 rotation = player_->GetRotation();
	PlayerPos.x = max(PlayerPos.x, -PlayerMoveLimX);
	PlayerPos.x = min(PlayerPos.x, +PlayerMoveLimX);
	PlayerPos.y = max(PlayerPos.y, -PlayerMaxMoveLimY);//���ɍs����͈�
	PlayerPos.y = min(PlayerPos.y, +PlayerMinMoveLimY);//��ɍs����͈�
	PlayerPos.z = max(PlayerPos.z, -PlayerMaxMoveLimZ);
	PlayerPos.z = min(PlayerPos.z, +PlayerMinMoveLimZ);
	player_->SetPosition(PlayerPos);
	//----------���ړ�����

	//------------------���v���C���[�ړ����p��
	if (cInput->LeftMove() || cInput->RightMove() || cInput->UpMove() || cInput->DownMove())// inputQ || inputZ ||
	{
		//------�v���C���[�������ړ�------//
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
	//���[�[�[�[�邽���މ��@���͕�������
	if (pDamFlag == true) {

		if (--pShakeTimer_ >= 0) {// 0�܂Ō��炷			
			//DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

			input->PadVibration();

			//pos�h�炷
			XMFLOAT3 pos = player_->GetPosition();
			randShakeNow = 8 + 1;//a~b
			pos.x = pos.x + rand() % randShakeNow - 4;//a~b�܂ł�randShakeNow�̍ő�l���甼���������ĕ��̐����܂ނ悤��
			pos.y = pos.y + rand() % randShakeNow - 4;
			player_->SetPosition(pos);

			if (pShakeTimer_ <= 0) {
				input->PadVibrationDef();
				pDamFlag = false;
			}//0�Ȃ����炭�炢��ԉ���
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

	// ���S����̋�
	cameraMoveVel.x = float(input->GetMousePos().x) - centerPos.x;
	cameraMoveVel.y = float(input->GetMousePos().y) - centerPos.y;

	input->SetMousePos((int)centerPos.x, (int)centerPos.y);
}

void GamePlayScene::UpdateCamera()
{
	// �J�����̋���
	constexpr float camLen = 64.f;
	// �J�����̍���
	//constexpr float camHeight = camLen * 10.5f;

	// ���@����J���������_�܂ł̋���
	constexpr float player2targetLen = camLen * 2.f;

	// ���@�̎����x�N�g��
	{
		//���x
		const float camMoveVel = 0.05f;

		XMFLOAT3 rota = player_->GetRotation();

		// �}�E�X�̉�����(X)�̈ړ����J�����̏c����(Y)�̉�]�ɂȂ�
		rota.x += cameraMoveVel.y * camMoveVel;
		// �}�E�X�̏c����(Y)�̈ړ����J�����̉�����(X)�̉�]�ɂȂ�
		rota.y += cameraMoveVel.x * camMoveVel;

		player_->SetRotation(rota);
	}
}

void GamePlayScene::PadStickCamera()
{
	//�p�b�h�E�X�e�B�b�N�ŃJ�������_
	Input* input = Input::GetInstance();

	//���x
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

	float NowBoHp = charParameters->GetNowBoHp();//���݂̂ڂ�HP�擾
	float BossDefense = boss_.front()->GetBossDefense();//�{�X�h��͎擾 �擪�v�f
	float NowpHp = charParameters->GetNowpHp();//���@�̗͎擾
	float pBulPow = player_->GetpBulPow();//���@�e�З�

	//------------------------------�������蔻��ZONE��-----------------------------//
	//[���@�̒e]��[�{�X]�̓����蔻��
	if (sEnemyMurdersNum >= BossTermsEMurdersNum&& BeforeBossAppearFlag == true) {
		{

			Sphere pBulForm;//��

			for (auto& pb : player_->GetBullets()) {
				if (!pb->GetAlive())continue;//����ł���X�L�b�v
				pBulForm.center = XMLoadFloat3(&pb->GetPosition());
				pBulForm.radius = pb->GetScale().x;

				// �Փ˔��������
				for (auto& bo : boss_) {
					if (!bo->GetAlive())continue;
					Sphere enemyForm;
					enemyForm.center = XMLoadFloat3(&bo->GetPosition());
					enemyForm.radius = bo->GetScale().x;

					// ���������������
					if (Collision::CheckSphere2Sphere(pBulForm, enemyForm)) {
						XMFLOAT3 boPos = bo->GetPosition();
						
						pb->SetAlive(false);

						//bo->SetColor({ 1,0,0,1 });
						if ((NowBoHp - (pBulPow - BossDefense))>0) {
							ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
						}
						NowBoHp -= (pBulPow - BossDefense);
						charParameters->SetNowBoHp(NowBoHp);//�{�XHP�Z�b�g

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.4f, 0);

						if (NowBoHp <= 0) {
							GameSound::GetInstance()->PlayWave("bossdeath.wav", 0.3f, 0);
							bo->SetAlive(false);
						}

						break;
					}
				}
			}
			////�{�X�����TRUE�@��������FALSE�@���Ȃ���ENPTY
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
			// �{�X������
			boss_.erase(std::remove_if(boss_.begin(), boss_.end(),
				[](const std::unique_ptr <Boss>& i) {return !i->GetAlive() && i->GetBullets().empty(); }), boss_.end());
		}
	}

	//[���@�̒e]��[�G���G]�����蔻��
	{

		Sphere pBulForm;

		for (auto& pb : player_->GetBullets()) {
			if (!pb->GetAlive())continue;
			pBulForm.center = XMLoadFloat3(&pb->GetPosition());
			pBulForm.radius = pb->GetScale().x;

			// �Փ˔��������
			for (auto& se : smallEnemys_) {
				if (!se->GetAlive())continue;
				Sphere smallenemyForm;
				smallenemyForm.center = XMLoadFloat3(&se->GetPosition());
				smallenemyForm.radius = se->GetScale().x + 5.f;//�]�T���������镪�{

				// ���������������
				if (Collision::CheckSphere2Sphere(pBulForm, smallenemyForm)) {
					GameSound::GetInstance()->PlayWave("se_baaan1.wav", 0.1f, 0);
					sEnemyMurdersNum++;//���j��
					// �p�[�e�B�N���̔���
					XMFLOAT3 sePos = se->GetPosition();
					ParticleManager::GetInstance()->CreateParticle(sePos, 300, 80, 5);
					se->SetAlive(false);
					pb->SetAlive(false);
					break;
				}
			}
		}
	}

	//���Ńt���O�������炻�̎G���G�͎����Ĕq����
	smallEnemys_.remove_if([](std::unique_ptr<SmallEnemy>& smallEnemy) {
		return !smallEnemy->GetAlive();
		});

	//[���@]��[�{�X�e]�̓����蔻��
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
						NowpHp -= eBulPower;//���@�_���[�W
						charParameters->SetNowpHp(NowpHp);//�v���C���[HP�Z�b�g

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
						if (NowpHp <= 0) {//HP0�Ŏ��S
							PlayerDeath();
						}
						break;
					}

				}
			}
		}


	}

	//[�G���G�e]��[���@]�̓����蔻��
	{
		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z + 2;

		if (player_->GetAlive()) {
			for (auto& se : smallEnemys_) {
				if (!se->GetAlive())continue;
				for (auto& seb : se->GetBullets()) {//seb �G���G�e
					Sphere seBulForm;
					seBulForm.center = XMLoadFloat3(&seb->GetPosition());
					seBulForm.radius = seb->GetScale().z + 1;//�]�T

					if (Collision::CheckSphere2Sphere(playerForm, seBulForm)) {
						float seBulPow = se->GetBulPow();//�G���G�ʏ�e�З�
						pDamFlag = true;
						NowpHp -= seBulPow;//���@�_���[�W
						charParameters->SetNowpHp(NowpHp);//�{�XHP�Z�b�g

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						if (NowpHp <= 0) {//HP0�Ŏ��S
							PlayerDeath();
						}
						break;
					}

				}
			}
		}
	}
	//------------------------------�������蔻��ZONE��-----------------------------//
}

void GamePlayScene::PauseConti()
{
	Input* input = Input::GetInstance();
	//�������u��
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//�p�b�h�����Ă����
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//�I�𒆃T�C�Y�ł�����
	sp_continuation->SetSize({ PauseSelectSize,100.f });
	sp_continuation->TransferVertexBuffer();

	if (TriggerDown || PadTriggerDown) {//1�����͑I��
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 1;
	}
	if (TriggerUp || PadTriggerUp) {//���2
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 2;
	}

	//�p��
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
	//�p�b�h�����Ă����
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//�I�𒆃T�C�Y�ł�����
	sp_operation->SetSize({ PauseSelectSize,100.f });
	sp_operation->TransferVertexBuffer();

	//��������J���ĂȂ��Ƃ��̂�
	if (OperWindOpenFlag == false)
	{
		if (TriggerDown || PadTriggerDown) {//����2
			sp_operation->SetSize({ PauseSelectSizeDef,100.f });
			sp_operation->TransferVertexBuffer();
			PauseNowSelect = 2;
		}
		if (TriggerUp || PadTriggerUp) {//���0
			sp_operation->SetSize({ PauseSelectSizeDef,100.f });
			sp_operation->TransferVertexBuffer();
			PauseNowSelect = 0;
		}
	}
	//���������ʊJ��
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
	//�p�b�h�����Ă����
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));

	//�I�𒆃T�C�Y�ł�����
	sp_gotitle->SetSize({ PauseSelectSize,100.f });
	sp_gotitle->TransferVertexBuffer();
	if (TriggerDown || PadTriggerDown) {//����0
		sp_gotitle->SetSize({ PauseSelectSizeDef,100.f });
		sp_gotitle->TransferVertexBuffer();
		PauseNowSelect = 0;
	}
	if (TriggerUp || PadTriggerUp) {//���1
		sp_gotitle->SetSize({ PauseSelectSizeDef,100.f });
		sp_gotitle->TransferVertexBuffer();
		PauseNowSelect = 1;
	}

	//�^�C�g���֖߂�
	if (TriggerEnter || PadTriggerA)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		input->PadVibration();//�U��
		// ������~
		GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");
		//�V�[���؂�ւ�
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
	//�p�b�h�g���K�[
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));
	input->PadVibrationDef();
	// �}�E�X���̍X�V
	UpdateMouse();

	/*
	////�I�𒆕\���@�f�o�b�O�p
	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", PauseNowSelect);
		DebugText::GetInstance()->Print(tmp, 430, 460, 5);
	}*/

	//�����o�֐��|�C���^�Ή������I��
	if (PauseNowSelect == 0) { pFunc = &GamePlayScene::PauseConti; }
	if (PauseNowSelect == 1) { pFunc = &GamePlayScene::PauseOper; }
	if (PauseNowSelect == 2) { pFunc = &GamePlayScene::PauseGoTitle; }

	//�����o�֐��|�C���^�Ăяo��
	(this->*pFunc)();

	//����
	WaitKey0++;//�����{�^���łƂ����悤��
	//���������ʌ��Ă�Ƃ��͉����Ă����Ȃ�
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
	//�p�b�h�g���K�[
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));

	if (Trigger0 || PadTriggerStart) {
		PauseFlag = true;
	}
	if (PauseFlag == true) {
		Pause();
	}

	//�|�[�Y�łȂ��Ƃ��`
	//���̊O�̓|�[�Y�������s
	if (PauseFlag == false)
	{

		Input* input = Input::GetInstance();
		DxBase* dxBase = DxBase::GetInstance();

		CharParameters* charParameters = CharParameters::GetInstance();
		float NowBoHp = charParameters->GetNowBoHp();//���݂̂ڂ�HP�擾
		float BossDefense = boss_.front()->GetBossDefense();//�{�X�h��͎擾
		float NowpHp = charParameters->GetNowpHp();//���@�̗͎擾

		//�L�[���쉟���Ă����
		// ���W����
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
		//�������u��
		const bool TriggerM = input->TriggerKey(DIK_M);
		const bool TriggerE = input->TriggerKey(DIK_E);
		const bool TriggerR = input->TriggerKey(DIK_R);
		const bool Trigger2 = input->TriggerKey(DIK_2);

		float pMaxHp = charParameters->GetpMaxHp();
		float boMaxHp = charParameters->GetboMaxHp();
		if (TriggerR) {//�f�o�b�N�p�@�K���Ɂ@�����͏���
			camera->SetTarget({  });
			camera->SetEye({  });
			player_->SetAlive(true);
			NowpHp = pMaxHp;
			boss_.front()->SetAlive(true);
			NowBoHp = boMaxHp;
			sEnemyMurdersNum = 0;
			BossEnemyAdvent = false;

			// �J����re�Z�b�g
			//Object3d::SetCamera(camera.get());
		}
		
		////�^�Q�ړ�
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

		//�G��HP�o�[
		if (BossEnemyAdvent == true)
		{
			charParameters->boHpSizeChange();
		}

		//���@��HP�o�[
		charParameters->pHpSizeChange();

		charParameters->BarGetDislodged();

		//�V����]
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

		if (BossEnemyAdvent == false)
		{
			//������������
			if (SEneAppCount == 0) {
				//�G���G����
				SmallEnemyAppear();
				//�ĂуJ�E���g�ł���悤�ɏ�����
				SEneAppCount = SEneAppInterval;
			}
		}
		//�G���G�J�E���g���f�N�������g
		SEneAppCount--;

		//----------------���V�[���؂�ւ��֘A��----------------//
		//�G���j�ŃN���A
		if (!boss_.front()->GetAlive()) {
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGM���
			BaseScene* scene = new ClearScene();
			sceneManager_->SetNextScene(scene);
		}
		//���@HP0�ŃQ�[���I�[�o�[
		if (!player_->GetAlive()) {
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGM���
			BaseScene* scene = new GameOver();
			sceneManager_->SetNextScene(scene);
		}
		//----------------���V�[���؂�ւ��֘A��---------------//

		//�o�b�N�X�v���C�g��
		//SPmove SPbackmoveobj;
		//for (int i = 0; i < 1; i++)
		//{
		//	XMFLOAT3 position = sprite_back->GetPosition();

		//	position.x += 5;
		//	if (position.x == 0) { position.x = -11400; }

		//	sprite_back->SetPosition(position);
		//}

		//���������N�[���^�C��
		CoolTime();
		CollisionAll();
		DrawUI();
		//�p�b�h�E�X�e�B�b�N�J�������_
		PadStickCamera();

		// �}�E�X���̍X�V
		UpdateMouse();
		// �J�����̍X�V
		camera->Update();
		UpdateCamera();
		// �p�[�e�B�N���X�V
		ParticleManager::GetInstance()->Update();

		if (pRotDef == false) { //��x����
			input->PadVibrationDef();
			player_->SetRotation({});
			pRotDef = true;
		}

		//�v���C���[�ړ�
		PlayerMove();

		//// �X�v���C���Ȑ��ňړ�
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

		//	// �x�N�^�[���t���[�g�ɕϊ�
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
		//�G���G�X�V
		if (BossEnemyAdvent == false)
		{
			for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
				smallEnemy->Update();
			}
		}

		//���j���B���Ń{�X��
		if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
			//�c���Ă���G���G�͂�������Ȃ�
			for (auto& se : smallEnemys_) {//����G���G�̕�����
				se->SetAlive(false);//����
			}

			if (BeforeBossAppearFlag == false)
			{
				BeforeBossAppear();
			}
			//�{�X��O�̉��o
			if (BeforeBossAppearFlag == true)
			{//���o�I�������
				//�{�X��˓��̂��m�点�ł�
				BossEnemyAdvent = true;
				for (std::unique_ptr<Boss>& boss : boss_) {
					boss->Update();//�G�X�V
				}
			}
		}
		// FBX Update
		//fbxObject_1->Update();

		//�X�v���C�g�X�V
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
		//�G��HP�o�[
		if (BossEnemyAdvent == true)
		{
			charParameters->boHpUpdate();
		}

		player_->Update();
	}//�����܂Ń|�[�Y���ĂȂ��Ƃ�

}

void GamePlayScene::Draw()
{

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();

	CharParameters* charParameters = CharParameters::GetInstance();
	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
	//sprite_back->Draw();


	//3d�I�u�W�F�`��O����
	Object3d::PreDraw(DxBase::GetInstance()->GetCmdList());

	//�G���G
	for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
		smallEnemy->Draw();
	}

	//�G�`��
	if (sEnemyMurdersNum >= BossTermsEMurdersNum && BeforeBossAppearFlag == true) {
		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Draw();
		}
	}

	//3d�I�u�W�F�`��
	object3d_1->Draw();
	obj_worlddome->Draw();
	//obj_sword->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();

	//���L�����`��
	player_->Draw();

	//smallEnemy_->Draw();

	// FBX3d�I�u�W�F�N�g�`��
	//fbxObject_1->Draw(cmdList);

	// �p�[�e�B�N���`��
	DxBase* dxBase = DxBase::GetInstance();
	ParticleManager::GetInstance()->Draw(dxBase->GetCmdList());

	//3d�I�u�W�F�`��㏈��
	Object3d::PostDraw();

	// �S�D�`��R�}���h��������

	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//    DrawObject3d(&object3ds[i], dxBase->GetCmdList(), basicDescHeap.Get(), vbView, ibView,
	//        CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeap->GetGPUDescriptorHandleForHeapStart(), constantBufferNum, dxBase->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)),
	//        indices, _countof(indices));
	//}

	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();

	//---------------����O�X�v���C�g�`��
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
	else if (BossEnemyAdvent == true) { charParameters->boHpDraw(); }//�{�X�펞�̂ݕ\��

	if (OperWindOpenFlag == true) { sp_operation_wind->Draw(); }

	//�{�X��O �|�[�Y���͌����Ȃ�
	if (BeforeBossAppearNow == true && PauseFlag == false)
	{
		sp_beforeboss->Draw();
	}

	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
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
//	//mt3�X�v���C���Ȑ��̍l����
//	XMVECTOR position = 0.5 * ((2 * p1 + (-p0 + p2) * t) +
//		(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
//		(-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);
//
//	return position;
//}

void GamePlayScene::DrawUI()
{
	CharParameters* charParameters = CharParameters::GetInstance();
	//�����Ȃ���ɕ\��
	//DebugText::GetInstance()->Print("---PLAYSCENE---", 100, 70, 2);
	//DebugText::GetInstance()->Print("[LEFT CLICKorSPACEorPAD ZR] Firing", 100, 100, 2);
	//DebugText::GetInstance()->Print("[WASD&QZorGAMEPAD:STICK]PlayerMove", 100, 130, 2);
	//DebugText::GetInstance()->Print("[ALLOWorMOVE MOUSEorJ,K,L,I] PlayerRot", 100, 160, 2);
	//DebugText::GetInstance()->Print("[ESC] CLOSE WINDOW", 100, 190, 2);
	DebugText::GetInstance()->Print("Player HP", 150, 610, 2);
	//{
	////���@���W
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

	//���Ԍv��
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

	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {//�{�X�펞�̂�
		DebugText::GetInstance()->Print("Boss HP", 500, 10, 2);
		//DebugText::GetInstance()->Print("!!!Boss!!!", 100, 415, 3);
	}
	//else {//�{�X�킶��Ȃ��Ƃ��̂ݕ\��
	//	//�G���G���j���֘A
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
