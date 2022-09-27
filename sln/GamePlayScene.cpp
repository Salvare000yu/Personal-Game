#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "DxBase.h"
#include "EndScene.h"
#include "FbxObject3d.h"
#include "Collision.h"

#include "safe_delete.h"

#include <DirectXMath.h>

using namespace DirectX;

void GamePlayScene::Initialize()
{
#pragma region �`�揉��������

	camera.reset(new Camera(WinApp::window_width, WinApp::window_height));

	camera->SetTarget({ 0,50,-200 });
	camera->SetEye({ 0,48,-210 });

	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());
	// �J�����Z�b�g
	Object3d::SetCamera(camera.get());
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	FbxObject3d::SetCamera(camera.get());

	//�g����`�Ƃ��@�������Ƃ���
	time = frame / 60.f;	// 60fps�z��

	//------obj���烂�f���f�[�^�ǂݍ���---
	model_1.reset(Model::LoadFromOBJ("ground"));
	mod_worlddome.reset(Model::LoadFromOBJ("skydome"));
	mod_sword.reset(Model::LoadFromOBJ("chr_sword"));
	mod_kaberight.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_smallenemy.reset(Model::LoadFromOBJ("SmallEnemy"));
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
	//smallEnemy_ = new SmallEnemy();
	//���낢��L����������
	player_->Initialize();
	player_->SetModel(mod_player.get());
	player_->SetPBulModel(mod_playerbullet.get());
	//smallEnemy_->Initialize();

	enemy_.emplace_front();
	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		enemy=std::make_unique<Enemy>();
		enemy->Initialize();
		enemy->SetModel(mod_enemy.get());
		enemy->SetEBulModel(mod_enemybullet.get());
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
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav",0.5, XAUDIO2_LOOP_INFINITE);
	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// -----------------�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");

	// �X�v���C�g�̐���
	sprite_back.reset(Sprite::Create(1, DirectX::XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_guide.reset(Sprite::Create(2, DirectX::XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);
	//�X�v���C�g�|�W�V����
	sprite_back->SetPosition({ -11400,0,0 });
	sp_guide->SetPosition({ 0,0,0 });

	//�X�v���C�g�T�C�Y
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});

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

	//�G���G�o�^
	smallEnemys_.push_back(std::move(madeSmallEnemy));
}

void GamePlayScene::Update()
{

	Input* input = Input::GetInstance();

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
	//�������u��
	const bool TriggerJ = input->TriggerKey(DIK_J);//�G�o����(�o���֐���)
	const bool TriggerM = input->TriggerKey(DIK_M);
	const bool TriggerK = input->TriggerKey(DIK_K);
	const bool TriggerE = input->TriggerKey(DIK_E);
	const bool TriggerR = input->TriggerKey(DIK_R);
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	const bool Trigger2 = input->TriggerKey(DIK_2);
	//�p�b�h�����Ă����
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	//--------------------���ړ�����

	const float CameraTagMoveLimX = 190;
	const float CameraEyeMoveLimX = 190;

	const float CameraMaxTagMoveLimY = 90;//Y�ő�^�Q�@���ɂǂꂾ���s���邩�Ȃ񂩋t
	const float CameraMaxEyeMoveLimY = 92;//Y�ő�A�C
	const float CameraMinTagMoveLimY = 210;//Y�ŏ��^�Q�@��ɂǂꂾ���s���邩�Ȃ񂩋t
	const float CameraMinEyeMoveLimY = 208;//Y�ŏ��A�C

	const float CameraMaxTagMoveLimZ = 320;//��ő�^�Q
	const float CameraMaxEyeMoveLimZ = 330;//��ő�A�C
	const float CameraMinTagMoveLimZ = 170;//�O�ŏ��^�Q
	const float CameraMinEyeMoveLimZ = 160;//�O�ŏ��A�C

			//------���^�[�Q�b�g
	XMFLOAT3 target_moved = camera->GetTarget();
	target_moved.x = max(target_moved.x, -CameraTagMoveLimX);
	target_moved.x = min(target_moved.x, +CameraTagMoveLimX);
	target_moved.y = max(target_moved.y, -CameraMaxTagMoveLimY); //���ɂǂꂾ���s���邩�Ȃ񂩋t
	target_moved.y = min(target_moved.y, +CameraMinTagMoveLimY);
	target_moved.z = max(target_moved.z, -CameraMaxTagMoveLimZ);
	target_moved.z = min(target_moved.z, +CameraMinTagMoveLimZ);//�O
	camera->SetTarget(target_moved);
	//------���^�[�Q�b�g
	//------���߁I
	XMFLOAT3 eye_moved = camera->GetEye();
	eye_moved.x = max(eye_moved.x, -CameraEyeMoveLimX);
	eye_moved.x = min(eye_moved.x, +CameraEyeMoveLimX);
	eye_moved.y = max(eye_moved.y, -CameraMaxEyeMoveLimY); //��ɂǂꂾ���s���邩�Ȃ񂩋t
	eye_moved.y = min(eye_moved.y, +CameraMinEyeMoveLimY);
	eye_moved.z = max(eye_moved.z, -CameraMaxEyeMoveLimZ);//��
	eye_moved.z = min(eye_moved.z, +CameraMinEyeMoveLimZ);//�O
	camera->SetEye(eye_moved);
	//------���߁I
//--------------------���ړ�����

	if (inputW || inputS || inputA || inputD || inputQ || inputZ || PadInputUP || PadInputDOWN || PadInputLEFT || PadInputRIGHT)
	{

		//------�v���C���[�������ړ�------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 2;

		if (inputS || PadInputDOWN) {
			// �J�������o�b�N������
			camera->MoveEyeVector(XMFLOAT3(0, 0, -moveSpeed));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z - moveSpeed));

		}
		if (inputW || PadInputUP) {
			// �J������O�i������
			camera->MoveEyeVector(XMFLOAT3(0, 0, +moveSpeed));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z + moveSpeed));

		}
		if (inputA || PadInputLEFT) {
			// �J���������i������
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
			// �J�������E�i������
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
			// �J�������㏸������
			camera->MoveEyeVector(XMFLOAT3(0, moveSpeed, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y + moveSpeed, camera->GetTarget().z));

		}
		if (inputZ) {
			// �J���������~������
			camera->MoveEyeVector(XMFLOAT3(0, -moveSpeed, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y - moveSpeed, camera->GetTarget().z));

		}
	}

	if (TriggerR) {
		camera->SetTarget({ 0,50,-200 });
		camera->SetEye({ 0,48,-210 });
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

	//���@�̒e�ƓG�̓����蔻��
	{
		Sphere pBulForm;

		for (auto& pb : player_->GetBullets()) {
			if (!pb->GetAlive())continue;
			pBulForm.center = XMLoadFloat3(&pb->GetPosition());
			pBulForm.radius = pb->GetScale().x;

			// �Փ˔��������
			for (auto& e : enemy_) {
				if (!e->GetAlive())continue;
				Sphere enemyForm;
				enemyForm.center = XMLoadFloat3(&e->GetPosition());
				enemyForm.radius = e->GetScale().x;

				// ���������������
				if (Collision::CheckSphere2Sphere(pBulForm, enemyForm)) {
					e->SetAlive(false);
					pb->SetAlive(false);
					break;
				}
			}
		}

		//�G�����TRUE�@��������FALSE�@���Ȃ���ENPTY
		if (!enemy_.empty())
		{
			if (enemy_.front()->GetAlive()) {
				DebugText::GetInstance()->Print("TRUE", 200, 190, 2);
			}
			else {
				DebugText::GetInstance()->Print("FALSE", 200, 190, 2);
			}
		}
		else {
			DebugText::GetInstance()->Print("empty", 200, 190, 2);
		}

		// �G������
		enemy_.erase(std::remove_if(enemy_.begin(), enemy_.end(),
			[](const std::unique_ptr <Enemy>& i) {return !i->GetAlive() && i->GetBullets().empty(); }), enemy_.end());
	}

	//���@�̒e�ƎG���G�����蔻��
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
				smallenemyForm.radius = se->GetScale().x;

				// ���������������
				if (Collision::CheckSphere2Sphere(pBulForm, smallenemyForm)) {
					se->SetAlive(false);
					pb->SetAlive(false);
					break;
				}
			}
		}

		//// �G���G������
		//smallEnemys_.erase(std::remove_if(smallEnemys_.begin(), smallEnemys_.end(),
		//	[](const std::unique_ptr <GamePlayScene>& i) {return !i->GetAlive() && i->GetSmallEnemys().empty(); }), smallEnemys_.end());

	}

	//���Ńt���O�������炻�̎G���G�͎����Ĕq����
	smallEnemys_.remove_if([](std::unique_ptr<SmallEnemy>& smallEnemy) {
		return !smallEnemy->GetAlive();
		});

	//�V����]
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y += 0.3f;
		obj_worlddome->SetRotation({ rotation });

	}

	//������������
	if (SEneAppCount == 0) {
		//�G���G����
		SmallEnemyAppear();
		//�ĂуJ�E���g�ł���悤�ɏ�����
		SEneAppCount = SEneAppInterval;
	}

	//�G���G�J�E���g���f�N�������g
	SEneAppCount--;

	//�G���G�X�V
	for (std::unique_ptr<SmallEnemy>& smallEnemy : smallEnemys_) {
		smallEnemy->Update();
	}

	//�G�X�V
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->Update();
	}

	//if (Trigger0)     // �X�y�[�X�L�[��������Ă�����
	//{
	//	//�V�[���؂�ւ�
	//	BaseScene* scene = new EndScene();
	//	sceneManager_->SetNextScene(scene);
	//}

	//�o�b�N�X�v���C�g��
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
	camera->Update();

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

	player_->Update();
	//smallEnemy_->Update();
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
	for (std::unique_ptr<Enemy>& enemy : enemy_) {
		enemy->Draw();
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

	//------����O�X�v���C�g�`��
	sp_guide->Draw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
   // sprite->Draw();

}

void GamePlayScene::DrawUI()
{
}
