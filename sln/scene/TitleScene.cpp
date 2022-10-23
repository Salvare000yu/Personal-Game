#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"

#include "../PostEffect.h"

//PostEffect* postEffect = nullptr;

using namespace DirectX;

void TitleScene::Initialize()
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
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");

	// �X�v���C�g�̐���
	sprite1.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
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

	//�|�X�g�G�t�F�N�g�p�e�N�X�`���ǂݍ���
	//SpriteBase::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	//�|�X�g�G�t�F�N�g������
//	postEffect = new PostEffect();
////	postEffect->Initialize();//100, { 0.0f, 0.0f }, false, false��������
//	postEffect->SetSize({ 500.0f,500.0f });
//	postEffect->SetPosition({ 10,10,0 });
//	postEffect->TransferVertexBuffer();
#pragma endregion �`�揉��������
}

void TitleScene::Finalize()
{

	//�X�v���C�g���
	//delete sprite;

	//3d�I�u�W�F�N�g���
	//delete object3d_1;
	//delete object3d_2;
	//delete object3d_3;
	//delete model_1;
	//delete model_2;

//	delete postEffect;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	//�������u�Ԃ̂�
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//�p�b�h�������u��
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	input->PadVibration();
	if (TriggerSPACE||PadTriggerA)     // �X�y�[�X�L�[��������Ă�����
	{
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}

	//�Z���N�g����U������������
	if (--VibCount == 0) {
		input->PadVibrationDef();
	}

	DrawUI();

	//�X�v���C�g�X�V
	sprite1->Update();
	//postEffect->Update();

}

void TitleScene::Draw()
{
	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//// �X�v���C�g�`��
	sprite1->Draw();

}

void TitleScene::DrawUI()
{
	DebugText::GetInstance()->Print("[SPACEorGAMEPAD:A] PLAYSCENE", 300, 100, 3.0f);
}
