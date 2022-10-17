#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "../DxBase.h"
#include "ClearScene.h"
#include "GameOver.h"
#include "FbxObject3d.h"
#include "Collision.h"

#include "../safe_delete.h"

#include <DirectXMath.h>

using namespace DirectX;

void GamePlayScene::Initialize()
{
#pragma region �`�揉��������

	//camera.reset(new Camera(WinApp::window_width, WinApp::window_height));
	camera.reset(new CameraTracking());

	//camera->SetTarget({ 0,50,-200 });
	//camera->SetEye({ 0,48,-210 });
	Object3d::SetCamera(camera.get());

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
	mod_smallenemy.reset(Model::LoadFromOBJ("smallenemy_kari"));
	mod_playerbullet.reset(Model::LoadFromOBJ("PlayerBul"));
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	mod_player.reset(Model::LoadFromOBJ("hiyoko"));
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
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
	obj_worlddome->SetScale({ 5.0f, 5.0f, 5.0f });
	obj_sword->SetScale({ 7.0f, 7.0f, 7.0f });
	obj_kaberight->SetScale({ 200.0f, 200.0f, 10.0f });
	obj_kabeleft->SetScale({ 200.0f, 200.0f, 10.0f });
	//------object3d�ʒu------//
	object3d_1->SetPosition({ 0,-150,0 });
	obj_worlddome->SetPosition({ 0,200,150 });
	obj_sword->SetPosition({ 0,50,0 });
	obj_kaberight->SetPosition({ 210,-200,0 });
	obj_kabeleft->SetPosition({ -210,-200,0 });
	//------object��]------//
	obj_kaberight->SetRotation({ 0,90,0 });
	obj_kabeleft->SetRotation({ 0,-90,0 });

	//���낢�됶��
	player_.reset( new Player());
	//���낢��L����������
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
		boss=std::make_unique<Boss>();
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
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav",0.2f, XAUDIO2_LOOP_INFINITE);
	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// -----------------�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/HPbar.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/HPbar_waku.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/playerHPbar.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/playerHPbar_waku.png");
	SpriteBase::GetInstance()->LoadTexture(7, L"Resources/Miscellaneous Enemy Meter.png");

	// �X�v���C�g�̐���
	sprite_back.reset(Sprite::Create(1, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_guide.reset(Sprite::Create(2, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbar.reset(Sprite::Create(3, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbarwaku.reset(Sprite::Create(4, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbar.reset(Sprite::Create(5, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbarwaku.reset(Sprite::Create(6, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_semeter.reset(Sprite::Create(7, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);
	//�X�v���C�g�|�W�V����
	sprite_back->SetPosition({ -11400,0,0 });
	sp_guide->SetPosition({ 0,0,0 });
	sp_enemyhpbar->SetPosition({ 140,-80,0 });
	sp_enemyhpbarwaku->SetPosition({ 140,-80,0 });
	sp_playerhpbar->SetPosition({ -30,500,0 });
	sp_playerhpbarwaku->SetPosition({ -30,500,0 });
	sp_semeter->SetPosition({ 1170,620,0 });

	//---------�X�v���C�g�T�C�Y---------//
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});
	sp_playerhpbarwaku->size_.x = PlayerMaxHP;
	sp_semeter->SetSize({ 70.f,70.f });
	sp_playerhpbarwaku->TransferVertexBuffer();
	sp_semeter->TransferVertexBuffer();

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
#pragma endregion �`�揉��������

	//int counter = 0; // �A�j���[�V�����̌o�ߎ��ԃJ�E���^�[
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

void GamePlayScene::PlayerDeath()
{
	GameSound::GetInstance()->PlayWave("playerdeath.wav", 0.3f, 0);
	player_->SetAlive(false);
}

void GamePlayScene::CoolTime()
{
	//���[�[�[�[�邽���މ��@���͕�������
	if (pDamFlag == true) {
		if (--pShakeTimer_ >= 0) {// 0�܂Ō��炷			
			DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);
			
			//eye�h�炷
			XMFLOAT3 eye =camera->GetEye();
			randShakeNow = 4 + 1;//1~4
			eye.x = eye.x +rand()% randShakeNow-2;//1~4�܂ł�randShakeNow�̍ő�l���甼���������ĕ��̐����܂ނ悤��
			eye.y = eye.y +rand()% randShakeNow-2;
			camera->SetEye(eye);

			if (pShakeTimer_ <= 0) {
				pDamFlag = false;
			}//0�Ȃ����炭�炢��ԉ���
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

void GamePlayScene::CollisionAll()
{
	//------------------------------�������蔻��ZONE��-----------------------------//
	//[���@�̒e]��[�{�X]�̓����蔻��
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
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
				smallenemyForm.radius = se->GetScale().x+7.f;//�]�T���������镪�{

				// ���������������
				if (Collision::CheckSphere2Sphere(pBulForm, smallenemyForm)) {
					GameSound::GetInstance()->PlayWave("se_baaan1.wav", 0.2f, 0);
					sEnemyMurdersNum++;//���j��
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

	//[���@]��[�G�e]�̓����蔻��
	{

		Sphere playerForm;
		playerForm.center = XMLoadFloat3(&player_->GetPosition());
		playerForm.radius = player_->GetScale().z;

		if (player_->GetAlive()) {
			for (auto& bo : boss_) {
				if (!bo->GetAlive())continue;
				for (auto& bob : bo->GetBullets()) {
					Sphere eBulForm;
					eBulForm.center = XMLoadFloat3(&bob->GetPosition());
					eBulForm.radius = bob->GetScale().z;

					if (Collision::CheckSphere2Sphere(playerForm, eBulForm)) {

						pDamFlag = true;
						NowPlayerHP -= eBulPower;//���@�_���[�W

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						bob->SetAlive(false);
						if (NowPlayerHP <= 0) {//HP0�Ŏ��S
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
		playerForm.radius = player_->GetScale().z;

		if (player_->GetAlive()) {
			for (auto& se : smallEnemys_) {
				if (!se->GetAlive())continue;
				for (auto& seb : se->GetBullets()) {//seb �G���G�e
					Sphere seBulForm;
					seBulForm.center = XMLoadFloat3(&seb->GetPosition());
					seBulForm.radius = seb->GetScale().z;

					if (Collision::CheckSphere2Sphere(playerForm, seBulForm)) {
						pDamFlag = true;
						NowPlayerHP -= seBulPower;//���@�_���[�W

						GameSound::GetInstance()->PlayWave("playerdam.wav", 0.1f, 0);
						seb->SetAlive(false);
						if (NowPlayerHP <= 0) {//HP0�Ŏ��S
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

void GamePlayScene::Update()
{

	Input* input = Input::GetInstance();
	DxBase* dxBase = DxBase::GetInstance();

	//�L�[���쉟���Ă����
	// ���W����
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

	const bool input3 = input->PushKey(DIK_3);
	//�������u��
	const bool TriggerM = input->TriggerKey(DIK_M);
	const bool TriggerE = input->TriggerKey(DIK_E);
	const bool TriggerR = input->TriggerKey(DIK_R);
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	const bool Trigger2 = input->TriggerKey(DIK_2);
	const bool TriggerESC = input->TriggerKey(DIK_ESCAPE);
	//�p�b�h�����Ă����
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	if (TriggerR) {//�f�o�b�N�p�@�K���Ɂ@�����͏���
		camera->SetTarget({ 0,50,-200 });
		camera->SetEye({ 0,48,-210 });
		player_->SetAlive(true);
		NowPlayerHP = PlayerMaxHP;
		boss_.front()->SetAlive(true);
		NowBossHP = BossMaxHP;
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
	
	//���������N�[���^�C��
	CoolTime();

	//�G��HP�o�[
	if (BossEnemyAdvent == true)
	{
		//�X�V
		sp_enemyhpbar->Update();
		sp_enemyhpbarwaku->Update();
		for (int i = 0; i < 1; i++)
		{
			//�T�C�Y�ύX
			sp_enemyhpbar->size_.x = NowBossHP;
			sp_enemyhpbar->TransferVertexBuffer();
		}
	}

	//���@��HP�o�[
	for (int i = 0; i < 1; i++)
	{
		sp_playerhpbar->size_.x = NowPlayerHP;
		sp_playerhpbar->TransferVertexBuffer();
	}
	//�T�C�Y�ύX�ɂ��Y��--�����������牼
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

	//�V����]
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

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
	//�G���G�X�V
	if (BossEnemyAdvent == false)
	{
		for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
			smallEnemy->Update();
		}
	}

	//���j���B���Ń{�X��
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		//�{�X��˓��̂��m�点�ł�
		BossEnemyAdvent = true;
		//�c���Ă���G���G�͂�������Ȃ�
		for (auto& se : smallEnemys_) {//����G���G�̕�����
			se->SetAlive(false);//����
		}

		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Update();//�G�X�V
		}
	}

	//----------------���V�[���؂�ւ��֘A��----------------//
	//�G���j�ŃN���A
	if(!boss_.front()->GetAlive()){
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

	CollisionAll();
	DrawUI();
	// �}�E�X���̍X�V
	UpdateMouse();
	// �J�����̍X�V
	camera->Update();
	UpdateCamera();

	//3dobjUPDATE
	object3d_1->Update();
	obj_worlddome->Update();
	obj_sword->Update();
	obj_kaberight->Update();
	obj_kabeleft->Update();

	// FBX Update
	//fbxObject_1->Update();

	//�X�v���C�g�X�V
	sprite_back->Update();
	sp_guide->Update();
	sp_playerhpbar->Update();
	sp_playerhpbarwaku->Update();
	sp_semeter->Update();

	player_->Update();

	//�I��
	if (TriggerESC) {
		WM_DESTROY;//�j�������E�B���h�ɑ��M
		PostQuitMessage(0);//WM_DESTROY�̉����A�I���v��
	}
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();

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
	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		for (std::unique_ptr<Boss>& boss : boss_) {
			boss->Draw();
		}
	}

	//3d�I�u�W�F�`��
	object3d_1->Draw();
	obj_worlddome->Draw();
	obj_sword->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();

	//���L�����`��
	player_->Draw();
	//smallEnemy_->Draw();

	// FBX3d�I�u�W�F�N�g�`��
	//fbxObject_1->Draw(cmdList);

	//�p�[�e�B�N���`��
	//particleMan->Draw(cmdList);

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
	sp_guide->Draw();
	sp_playerhpbar->Draw();
	sp_playerhpbarwaku->Draw();
	sp_semeter->Draw();
	if (BossEnemyAdvent == true) { sp_enemyhpbar->Draw(); sp_enemyhpbarwaku->Draw(); }//�{�X�펞�̂ݕ\��
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
   // sprite->Draw();

}

void GamePlayScene::DrawUI()
{
	//�����Ȃ���ɕ\��
	DebugText::GetInstance()->Print("---PLAYSCENE---", 100, 70, 2);
	DebugText::GetInstance()->Print("[LEFT CLICKorSPACEorPAD ZR] Firing", 100, 100, 2);
	DebugText::GetInstance()->Print("[WASD&QZorGAMEPAD:STICK]PlayerMove", 100, 130, 2);
	DebugText::GetInstance()->Print("[ALLOWorMOVE MOUSEorJ,K,L,I] PlayerRot", 100, 160, 2);
	DebugText::GetInstance()->Print("[ESC] CLOSE WINDOW", 100, 190, 2);
	DebugText::GetInstance()->Print("Player HP", 150, 610, 2);
	//if (NowBossHP == 0) {
	//	DebugText::GetInstance()->Print("crushing!", 100, 230, 3);
	//}
	//if (player_->GetAlive()) {
	//	DebugText::GetInstance()->Print("Alive", 100, 270, 3);
	//}
	//else { DebugText::GetInstance()->Print("GameOver", 100, 270, 3); }

	if (sEnemyMurdersNum >= BossTermsEMurdersNum) {
		DebugText::GetInstance()->Print("Boss HP", 500, 10, 2);
	}

	//�G���G���j���֘A
	{
		DebugText::GetInstance()->Print("[BossTerms]", 100, 400, 2);
		
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", BossTermsEMurdersNum);
		DebugText::GetInstance()->Print(tmp, 300, 390,3);
	}
	{
		DebugText::GetInstance()->Print("[Now DefeatedEnemy]", 100, 440, 2);

		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", sEnemyMurdersNum);
		DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	}
}
