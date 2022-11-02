#include "GameOver.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
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
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("D_rhythmaze_119.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameOver.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/GameOver_Retry.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/GameOver_GoTitle.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/StageSelect_Now.png");

	// �X�v���C�g�̐���
	gameover.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	sp_retry.reset(Sprite::Create(2, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	sp_gotitle.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	sp_Now.reset(Sprite::Create(4, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));

	//�X�v���C�g�|�W�V����
	//window�c���擾�������Ƃ��g��
	WinApp* winApp = WinApp::GetInstance();
	float WindowWidthHalf_spPos = (winApp->window_width / 2) - 100;
	float WindowHeightHalf_spPos = (winApp->window_height / 2) - 100;//-100�̓X�v���C�g���W�C���p�@�摜�T�C�Y200������100

	sp_retry->SetPosition({ WindowWidthHalf_spPos - 300,WindowHeightHalf_spPos / 2+300,0 });
	sp_gotitle->SetPosition({ WindowWidthHalf_spPos + 300,WindowHeightHalf_spPos / 2+300,0 });
	sp_Now->SetPosition({ sp_retry->GetPosition().x - 50,sp_retry->GetPosition().y - 50,0 });
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
}

void GameOver::Finalize()
{

}

void GameOver::Retry()
{
	Input* input = Input::GetInstance();
	//�������u��
	const bool TriggerRight = input->TriggerKey(DIK_RIGHT);
	const bool TriggerLeft = input->TriggerKey(DIK_LEFT);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//�p�b�h�����Ă����
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerLeft = input->TriggerButton(static_cast<int>(Button::LEFT));
	const bool PadTriggerRight = input->TriggerButton(static_cast<int>(Button::RIGHT));

	XMFLOAT3 SSOp0_1pos = sp_Now->GetPosition();
	if (WaitKeyEase < 20) { WaitKeyEase++; }//���t������

	if (WaitKeyEase >= 2)//�܂��Ă�����͎�t
	{
		if (TriggerRight || PadTriggerRight) {//1�����͑I��
			selectPattern_ = SelectPattern::goTitle;//[���g���C����^�C�g����]�ɕύX
		}
	}

	if ((TriggerEnter || PadTriggerA) && CursorMoveNowFlag == false)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}

	{//���֐���
	//�C�[�Y�l����ĖڕW���W������\��Ȃ�
		if ((SSOp0_1pos.x + (EaseVal - DecEaseVal)) > (sp_gotitle->GetPosition().x - 50))
		{
			CursorMoveNowFlag = false;//���R�ɂ��Ă悢
			EaseVal = EaseValDef;//�C�[�Y�l������
			WaitKeyEase = 0;//��t�҂����ɖ߂�
			NowSelect = 1;
			SSOp0_1Flag = false;
		}
	}

	{//�ړ�����
		//0����1�ֈړ�
		if (SSOp0_1Flag == true)
		{
			CursorMoveNowFlag = true;//�����Ă邩����̓_����
			SSOp0_1pos.x += EaseVal;
			EaseVal -= DecEaseVal;
			sp_Now->SetPosition({ SSOp0_1pos.x,sp_gotitle->GetPosition().y - 50,0 });
		}
	}

	//--------���Z���N�g�p�^�[��enum
	//�����������X�^�[�g��
	if (selectPattern_ == SelectPattern::goTitle) {

		selectPattern_ = SelectPattern::def;
		SSOp0_1Flag = true;

	}
	//--------���Z���N�g�p�^�[��enum
}
void GameOver::GoTitle()
{
	Input* input = Input::GetInstance();
	//�������u��
	const bool TriggerRight = input->TriggerKey(DIK_RIGHT);
	const bool TriggerLeft = input->TriggerKey(DIK_LEFT);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//�p�b�h�����Ă����
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	const bool PadTriggerLeft = input->TriggerButton(static_cast<int>(Button::LEFT));
	const bool PadTriggerRight = input->TriggerButton(static_cast<int>(Button::RIGHT));

	XMFLOAT3 SSOp1_0pos = sp_Now->GetPosition();
	if (WaitKeyEase < 20) { WaitKeyEase++; }//���t������

	if (WaitKeyEase >= 2)//�܂��Ă�����͎�t
	{
		if (TriggerLeft || PadTriggerLeft) {//1�����͑I��
			selectPattern_ = SelectPattern::retry;//[�^�C�g���ւ��烊�g���C]�ɕύX
		}
	}

	if ((TriggerEnter || PadTriggerA) && CursorMoveNowFlag == false)
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	{//���֐���
	//�C�[�Y�l����ĖڕW���W������\��Ȃ�
		if ((SSOp1_0pos.x - (EaseVal - DecEaseVal)) < (sp_retry->GetPosition().x - 50))
		{
			CursorMoveNowFlag = false;//���R�ɂ��Ă悢
			EaseVal = EaseValDef;//�C�[�Y�l������
			WaitKeyEase = 0;//��t�҂����ɖ߂�
			NowSelect = 0;
			SSOp0_1Flag = false;
		}
	}

	{//�ړ�����
		//0����1�ֈړ�
		if (SSOp0_1Flag == true)
		{
			CursorMoveNowFlag = true;//�����Ă邩����̓_����
			SSOp1_0pos.x -= EaseVal;
			EaseVal -= DecEaseVal;
			sp_Now->SetPosition({ SSOp1_0pos.x,sp_gotitle->GetPosition().y - 50,0 });
		}
	}

	//--------���Z���N�g�p�^�[��enum
	//�����������X�^�[�g��
	if (selectPattern_ == SelectPattern::retry) {

		selectPattern_ = SelectPattern::def;
		SSOp0_1Flag = true;

	}
	//--------���Z���N�g�p�^�[��enum
}

void GameOver::Update()
{
	Input* input = Input::GetInstance();
	//const bool input1 = input->TriggerKey(DIK_1);
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//�p�b�h�������u��
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	input->PadVibrationDef();

	//�����o�֐��|�C���^�Ή������I��
	if (NowSelect == 0) { pFunc = &GameOver::Retry; }
	if (NowSelect == 1) { pFunc = &GameOver::GoTitle; }

	//�����o�֐��|�C���^�Ăяo��
	(this->*pFunc)();

	//if (PadTriggerA|| TriggerEnter)     // 1��������Ă�����
	//{

	//	GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
	//	//�U��
	//	input->PadVibration();
	//	// ������~
	//	GameSound::GetInstance()->SoundStop("D_rhythmaze_119.wav");
	//	//�V�[���؂�ւ�
	//	BaseScene* scene = new TitleScene();
	//	sceneManager_->SetNextScene(scene);
	//}

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
	sp_retry->Update();
	sp_gotitle->Update();
	sp_Now->Update();

}

void GameOver::Draw()
{
	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
	gameover->Draw();
	sp_retry->Draw();
	sp_gotitle->Draw();
	sp_Now->Draw();
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
	//DebugText::GetInstance()->Print("[ENTERorPAD_A]:Title", 430, 100, 3);
}
