#include "GameOver.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
//#include "GamePlayScene.h"
#include "TitleScene.h"

using namespace DirectX;

void GameOver::Initialize()
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
	GameSound::GetInstance()->LoadWave("D_rhythmaze_119.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("D_rhythmaze_119.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameOver.png");

	// �X�v���C�g�̐���
	gameover.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // �X�v���C�g�̍��W�ύX
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);ClearScene
	//}

#pragma endregion �`�揉��������

	int counter = 0; // �A�j���[�V�����̌o�ߎ��ԃJ�E���^�[
}

void GameOver::Finalize()
{

	//�X�v���C�g���
	//delete sprite;

	//3d�I�u�W�F�N�g���
	//delete object3d_1;
	//delete object3d_2;
	//delete object3d_3;
	//delete model_1;
	//delete model_2;
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	//const bool input1 = input->TriggerKey(DIK_1);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//�p�b�h�������u��
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	input->PadVibrationDef();
	if (PadTriggerA|| TriggerEnter)     // 1��������Ă�����
	{
		//�U��
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	// ���W����
	//const bool inputUp = input->PushKey(DIK_UP);
	//const bool inputDown = input->PushKey(DIK_DOWN);
	//const bool inputRight = input->PushKey(DIK_RIGHT);
	//const bool inputLeft = input->PushKey(DIK_LEFT);
	
	DrawUI();

	//DebugText::GetInstance()->Print("nihon kougakuin!", 200, 200, 2.0f);

	////3dobj
	//object3d_1->Update();
	//object3d_2->Update();
	//object3d_3->Update();

	//�X�v���C�g�X�V
	gameover->Update();

}

void GameOver::Draw()
{
	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
	gameover->Draw();

	////3d�I�u�W�F�`��O����
	//Object3d::PreDraw();

	////3d�I�u�W�F�`��
	//object3d_1->Draw();
	//object3d_2->Draw();
	//object3d_3->Draw();

	////3d�I�u�W�F�`��㏈��
	//Object3d::PostDraw();

	// �S�D�`��R�}���h��������

	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//    DrawObject3d(&object3ds[i], dxBase->GetCmdList(), basicDescHeap.Get(), vbView, ibView,
	//        CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeap->GetGPUDescriptorHandleForHeapStart(), constantBufferNum, dxBase->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)),
	//        indices, _countof(indices));
	//}

	//// �X�v���C�g���ʃR�}���h
	//SpriteBase::GetInstance()->PreDraw();
	//SpriteBase(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
	//sprite->Draw();
}

void GameOver::DrawUI()
{
	DebugText::GetInstance()->Print("[ENTERorPAD_A]:Title", 430, 100, 3);
}
