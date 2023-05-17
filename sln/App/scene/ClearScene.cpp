#include "ClearScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
//#include "GamePlayScene.h"
#include "TitleScene.h"
#include "Timer.h"

using namespace DirectX;

void ClearScene::Initialize()
{
#pragma region �`�揉��������

	// �}�E�X�J�[�\����\��
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	////---obj���烂�f���f�[�^�ǂݍ���---
	//model_1 = Model::LoadFromOBJ("ground");
	//model_2 = Model::LoadFromOBJ("triangle_mat");
	////Model* model_3 = Model::LoadFromOBJ("chr_sword");
	////---3d�I�u�W�F�N�g����---
	//object3d_1 = Object3d::Create();
	//object3d_2 = Object3d::Create();
	//object3d_3 = Object3d::Create();
	////---3d�I�u�W�F�N�g��3d���f����R�Â���---
	//object3d_1->SetModel(model_1);
	//object3d_2->SetModel(model_2);
	//object3d_3->SetModel(model_2);

	//object3d_2->SetScale({ 20.0f, 20.0f, 20.0f });
	//object3d_3->SetScale({ 30.0f, 30.0f, 30.0f });

	//object3d_2->SetPosition({ 5,-1,5 });
	//object3d_3->SetPosition({ -5,-1,5 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("verROOP_tukawanakutemoiiYO.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("verROOP_tukawanakutemoiiYO.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameClear.png");

	// �X�v���C�g�̐���
	sprite.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));

#pragma endregion �`�揉��������
}

void ClearScene::Finalize()
{
}

void ClearScene::Update()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	if ((cInput->Decision()))     // �G���^�[�L�[��������Ă�����
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		//�U��
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("verROOP_tukawanakutemoiiYO.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	DrawUI();

	//�X�v���C�g�X�V
	sprite->Update();
}

void ClearScene::Draw()
{
}

void ClearScene::DrawUI()
{
	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
	sprite->Draw();
}