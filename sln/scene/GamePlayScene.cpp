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
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	//Model* model_3 = Model::LoadFromOBJ("chr_sword");
	//------3d�I�u�W�F�N�g����------//
	object3d_1.reset(Object3d::Create());
	obj_worlddome.reset(Object3d::Create());
	obj_sword.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_tunnel.reset(Object3d::Create());
	//------3d�I�u�W�F�N�g��3d���f����R�Â���------//
	object3d_1->SetModel(model_1.get());
	obj_worlddome->SetModel(mod_worlddome.get());
	obj_sword->SetModel(mod_sword.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_tunnel->SetModel(mod_tunnel.get());
	//------object3d�X�P�[��------//
	object3d_1->SetScale({ 80.0f, 20.0f, 500.0f });
	obj_worlddome->SetScale({ 50.0f, 50.0f, 50.0f });
	obj_sword->SetScale({ 7.0f, 7.0f, 7.0f });
	obj_kaberight->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_kabeleft->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_tunnel->SetScale({ 2.0f, 2.0f, 2.0f });
	//------object3d�ʒu------//
	object3d_1->SetPosition({ 0,-150,0 });
	obj_worlddome->SetPosition({ 0,200,150 });
	obj_sword->SetPosition({ 0,50,0 });
	obj_kaberight->SetPosition({ 310,-200,0 });
	obj_kabeleft->SetPosition({ -310,-200,0 });
	obj_tunnel->SetPosition({ 0,40,-170 });
	//------object��]------//
	obj_kaberight->SetRotation({ 0,90,0 });
	obj_kabeleft->SetRotation({ 0,-90,0 });
	obj_tunnel->SetRotation({ 0,-90,0 });

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
	for (std::unique_ptr<Boss>& boss : boss_)//�{�X
	{
		boss = std::make_unique<Boss>();
		boss->Initialize();
		boss->SetModel(mod_enemy.get());
		boss->SetBulModel(mod_enemybullet.get());
		boss->SetAimBulModel(mod_enemybullet.get());
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

	Pause* pause = Pause::GetInstance();
	pause->Initialize();
	// -----------------�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	//SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");
	SpriteBase::GetInstance()->LoadTexture(13, L"Resources/sight.png");
	SpriteBase::GetInstance()->LoadTexture(14, L"Resources/Before_Boss.png");
	SpriteBase::GetInstance()->LoadTexture(15, L"Resources/GameReady.png");
	SpriteBase::GetInstance()->LoadTexture(16, L"Resources/GameGO!.png");
	SpriteBase::GetInstance()->LoadTexture(17, L"Resources/BlackWindow.png");
	SpriteBase::GetInstance()->LoadTexture(18, L"Resources/dame_ef.png");

	// �X�v���C�g�̐���
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

	//�X�v���C�g�|�W�V����
	sprite_back->SetPosition({ -11400,0,0 });
	//sp_guide->SetPosition({ 0,0,0 });
	//sp_sight->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2,0 });
	sp_sight->SetPosition({ 0,0,0 });

	//�X�v���C�g�J���[
	sp_blackwindow->SetColor({ 1, 1, 1, 0 });

	//---------�X�v���C�g�T�C�Y---------//
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});

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
	charParameters->SetNowBoHp({ charParameters->GetboMaxHp() });//�{�X�̗�
	charParameters->SetNowpHp({ charParameters->GetpMaxHp() });//���@�̗�

	//������p�[�e�B�N�����폜����
	ParticleManager::GetInstance()->DeleteParticles();

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

	//�{�X���j�ŃN���A�@Update�����ƈ�u��ʌ������Ⴄ���炱����
	if (!boss_.front()->GetAlive()) {
		GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGM���
		BaseScene* scene = new ClearScene();
		sceneManager_->SetNextScene(scene);
	}
}

void GamePlayScene::PlayerMove()
{
	//Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

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
			isLMove = true;//���ړ���
		}

		if (cInput->RightMove()) {

			PlayerPos.x += moveSpeed;

			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}
			isRMove = true;//�E�ړ���
		}
	}
	else
	{
		isLMove = false;
		isRMove = false;
	}

	//���͂Ȃ��Ƃ��߂�
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
	//���[�[�[�[�邽���މ��@���͕�������
	if (pDamFlag == true) {

		//DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

		//�摜�������Ă�
		pDamCol.w -= 0.03;
		//0���傫���Ԃ��܂���������ĂȂ��Ƃ�
		if (pDamCol.w > 0 && DamEfRedFlag == false) {
			sp_dame_ef->Update();
		}
		else {
			//�J��Ԃ��Ȃ��悤��
			DamEfRedFlag = true;
		}
	}
	else {
		//�_���[�W�I�������Ԃ̃_���[�W�摜�F�߂�
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

	// ���S����̋�
	cameraMoveVel.x = float(input->GetMousePos().x) - centerPos.x;
	cameraMoveVel.y = float(input->GetMousePos().y) - centerPos.y;

	input->SetMousePos((int)centerPos.x, (int)centerPos.y);
}

void GamePlayScene::UpdateCamera()
{
	// �J�����̋���
	//constexpr float camLen = 64.f;
	// �J�����̍���
	//constexpr float camHeight = camLen * 10.5f;

	// ���@����J���������_�܂ł̋���
	//constexpr float player2targetLen = camLen * 2.f;

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
	if (sEnemyMurdersNum >= BossTermsEMurdersNum && BeforeBossAppearFlag == true) {
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

					if (bo->GetisDeath())continue;

					// ���������������
					if (Collision::CheckSphere2Sphere(pBulForm, enemyForm)) {
						XMFLOAT3 boPos = bo->GetPosition();

						pb->SetAlive(false);

						//bo->SetColor({ 1,0,0,1 });
						if ((NowBoHp - (pBulPow - BossDefense)) > 0) {
							ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
						}
						NowBoHp -= (pBulPow - BossDefense);
						charParameters->SetNowBoHp(NowBoHp);//�{�XHP�Z�b�g

						GameSound::GetInstance()->PlayWave("bossdam_1.wav", 0.4f, 0);

						if (NowBoHp <= 0) {
							GameSound::GetInstance()->PlayWave("bossdeath.wav", 0.3f, 0);
							bo->SetisDeath(true);
							//�c���Ă���G���G�͂�������Ȃ�
							for (auto& bob : bo->GetBullets()) {//����G���G�̕�����
								bob->SetAlive(false);//����
							}

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
						charParameters->SetispDam(true);
						charParameters->SetNowpHp(NowpHp);//�v���C���[HP�Z�b�g

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
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
						charParameters->SetispDam(true);//���@���炢
						charParameters->SetNowpHp(NowpHp);//�{�XHP�Z�b�g

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						break;
					}

				}
			}
		}
	}
	//------------------------------�������蔻��ZONE��-----------------------------//
}

bool GamePlayScene::GameReady()
{
	XMFLOAT4 ReadyCol = sp_ready->GetColor();
	XMFLOAT4 GOCol = sp_ready_go->GetColor();
	XMFLOAT2 GOSize = sp_ready_go->GetSize();
	XMFLOAT3 GOPos = sp_ready_go->GetPosition();
	//�v���C���[���Ń��f�B�[����Attack���Ȃ��悤�ɂ���
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
		//�A�^�b�N�J�n���Ă悫
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
	//�p�b�h�g���K�[
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));

	CharParameters* charParameters = CharParameters::GetInstance();

	if (pause->WaitKey0 < 10&& pause->GetPauseFlag() == false) {
		pause->WaitKey0++;//�|�[�Y������͑҂B1�t���ŊJ���ĕ����Ⴄ����2�񉟂����I�Ȋ����ɂȂ����Ⴄ
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
		UpdateMouse();//�|�[�Y���Ă�Ƃ����}�E�X�X�V�@����Pause�֐���

		if (pause->GetSceneChangeTitleFlag() == true) {
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();//�U��
			// ������~
			GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");
			//�V�[���؂�ւ�
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}

	}

	//�|�[�Y�łȂ��Ƃ��`
	//���̊O�̓|�[�Y�������s
	if (pause->GetPauseFlag() == false)
	{

		Input* input = Input::GetInstance();
		DxBase* dxBase = DxBase::GetInstance();

		CharParameters* charParameters = CharParameters::GetInstance();
		float NowBoHp = charParameters->GetNowBoHp();//���݂̂ڂ�HP�擾
		float BossDefense = boss_.front()->GetBossDefense();//�{�X�h��͎擾
		float NowpHp = charParameters->GetNowpHp();//���@�̗͎擾

		//�L�[���쉟���Ă����

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

		//���@��HP�o�[
		charParameters->pHpSizeChange();

		charParameters->BarGetDislodged();

		//�V����]
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

		if (player_->GetPHpLessThan0() == false)
		{
			//�v���C���[�ړ�-��ɏ����ƈړ��������Ȃ�
			PlayerMove();
		}

		DrawUI();
		//�p�b�h�E�X�e�B�b�N�J�������_
		PadStickCamera();

		// �}�E�X���̍X�V
		UpdateMouse();
		// �J�����̍X�V
		camera->Update();
		UpdateCamera();

		if (pRotDef == false) { //��x����
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

		//�X�v���C�g�X�V
		sprite_back->Update();
		//sp_guide->Update();
		charParameters->pHpUpdate();

		pause->SpUpdate();

		player_->Update();

		if (GameReady() == false)
		{
			PlayTimer();

			//if (TriggerR) {//�f�o�b�N�p�@�K���Ɂ@�����͏���
			//	camera->SetTarget({  });
			//	camera->SetEye({  });
			//	player_->SetAlive(true);
			//	NowpHp = pMaxHp;
			//	boss_.front()->SetAlive(true);
			//	NowBoHp = boMaxHp;
			//	sEnemyMurdersNum = 0;
			//	BossEnemyAdvent = false;

			//	// �J����re�Z�b�g
			//	//Object3d::SetCamera(camera.get());
			//}

			//�G��HP�o�[
			if (BossEnemyAdvent == true)
			{
				charParameters->boHpSizeChange();
			}

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

			//------�_���e��
			//�G���G�̌��e���v���C���[�̂����ꏊ�ɔ��ł���
			for (auto& se : smallEnemys_) {
				//�^�[�Q�b�g
				se->SetShotTag(player_.get());
			}
			for (auto& bo : boss_) {
				bo->SetShotTag(player_.get());
			}
			//------�_���e��
			
			//���@���Ŏ��S�m�F���������
			if (player_->GetpDeath() == true) {
				GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.3f, 0);
				player_->SetAlive(false);
			}
			//----------------���V�[���؂�ւ��֘A��----------------//
			//���@HP0�ŃQ�[���I�[�o�[
			if (!player_->GetAlive()) {
				GameSound::GetInstance()->SoundStop("E_rhythmaze_128.wav");//BGM���
				BaseScene* scene = new GameOver();
				sceneManager_->SetNextScene(scene);
			}
			//----------------���V�[���؂�ւ��֘A��---------------//

			//���������N�[���^�C��
			CoolTime();

			if (player_->GetPHpLessThan0() == false)
			{
				CollisionAll();
			}

			// �p�[�e�B�N���X�V
			ParticleManager::GetInstance()->Update();

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
			//�G��HP�o�[
			if (BossEnemyAdvent == true && NowBoHp > 0)
			{
				charParameters->boHpUpdate();
			}

		}

	}//�����܂Ń|�[�Y���ĂȂ��Ƃ�

	//-------��Ƀf�o�e�L��
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
	//obj_tunnel->Draw();

	//���L�����`��
	player_->Draw();

	//smallEnemy_->Draw();

	// FBX3d�I�u�W�F�N�g�`��
	//fbxObject_1->Draw(cmdList);

	// �p�[�e�B�N���`��
	DxBase* dxBase = DxBase::GetInstance();
	ParticleManager::GetInstance()->Draw(dxBase->GetCmdList());

	float NowBoHp = charParameters->GetNowBoHp();//���݂̂ڂ�HP�擾

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
	}//�{�X�펞�̂ݕ\��

	if (pause->GetOpWindOpenFlag() == true) { pause->SpOperWindDraw(); }

	//�{�X��O �|�[�Y���͌����Ȃ�
	if (BeforeBossAppearNow == true && pause->GetPauseFlag() == false)
	{
		sp_beforeboss->Draw();
	}

	//�J�n�O���̂�
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
	//�������Ń_���[�W���炢��ԉ��������炱�����ł����l
	if (charParameters->GetispDam() == false) {
		pDamFlag = false;
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

	//{//�U������
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%d", pShakeTimer_);
	//	DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	//}

	charParameters->DrawUI();
}

void GamePlayScene::PlayTimer()
{
	Pause* pause = Pause::GetInstance();
	//���Ԍv��
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