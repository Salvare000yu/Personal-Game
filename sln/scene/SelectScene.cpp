#include "SelectScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"
#include "WinApp.h"
#include "TitleScene.h"

#include "../PostEffect.h"

using namespace DirectX;

void SelectScene::Initialize()
{
	//window�c���擾�������Ƃ��g��
	WinApp* winApp = WinApp::GetInstance();

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("C_rhythmaze_125.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("C_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/StageSelect.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/StageSelect_oper.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/StageSelect_Start.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/StageSelect_Title.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/StageSelect_Now.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/operation_wind.png");

	// �X�v���C�g�̐���
	sp_stageselect.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SSoper.reset(Sprite::Create(2, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SSstart.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SStitle.reset(Sprite::Create(4, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_SSNow.reset(Sprite::Create(5, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation_wind.reset(Sprite::Create(6, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//�X�v���C�g�|�W�V����
	float WindowWidthHalf_spPos = (winApp->window_width / 2) - 100;
	float WindowHeightHalf_spPos = (winApp->window_height / 2) - 100;//-100�̓X�v���C�g���W�C���p�@�摜�T�C�Y200������100

	sp_SSoper->SetPosition({ WindowWidthHalf_spPos - 400,WindowHeightHalf_spPos,0 });
	sp_SSstart->SetPosition({ WindowWidthHalf_spPos,WindowHeightHalf_spPos,0 });
	sp_SStitle->SetPosition({ WindowWidthHalf_spPos + 400,WindowHeightHalf_spPos,0 });
}

void SelectScene::SelectOper()
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

	//�I�𒆉摜�\�� -50�͘g�̏c����
	sp_SSNow->SetPosition({ sp_SSoper->GetPosition().x - 50,sp_SSoper->GetPosition().y - 50,0 });

	if (TriggerRight || PadTriggerRight) {//1�����͑I��
		NowSelect = 1;
	}
	if (TriggerLeft || PadTriggerLeft) {
		NowSelect = 2;
	}

	//����������
	if (TriggerEnter || PadTriggerA)
	{
		OperWindOpenFlag = true;
		OperationWind();

		WaitKeyEnter++;
		if ((TriggerEnter || PadTriggerA) && WaitKeyEnter >= 2) {
			OperWindOpenFlag = false;
			WaitKeyEnter = 0;
		}

	}

}
void SelectScene::OperationWind()
{
	sp_operation_wind->Update();
}

void SelectScene::SelectStart()
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

	//�I�𒆉摜�\�� -50�͘g�̏c����
	sp_SSNow->SetPosition({ sp_SSstart->GetPosition().x - 50,sp_SSstart->GetPosition().y - 50,0 });

	if (TriggerRight || PadTriggerRight) {
		NowSelect = 2;
	}
	if (TriggerLeft || PadTriggerLeft) {
		NowSelect = 0;
	}

	//�v���C�V�[����
	if (TriggerEnter || PadTriggerA)
	{
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}
}
void SelectScene::SelectTitle()
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

	//�I�𒆉摜�\�� -50�͘g�̏c����
	sp_SSNow->SetPosition({ sp_SStitle->GetPosition().x - 50,sp_SStitle->GetPosition().y - 50,0 });

	if (TriggerRight || PadTriggerRight) {
		NowSelect = 0;
	}
	if (TriggerLeft || PadTriggerLeft) {
		NowSelect = 1;
	}

	//�^�C�g���V�[����
	if (TriggerEnter || PadTriggerA)
	{
		input->PadVibration();
		// ������~
		GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}
}

void SelectScene::Finalize()
{
}

void SelectScene::Update()
{
	Input* input = Input::GetInstance();

	//�������u�Ԃ̂�
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//�p�b�h�������u��
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));

	if (TriggerSPACE || PadTriggerA)     // �X�y�[�X�L�[��������Ă�����
	{
		//input->PadVibration();
		//// ������~
		//GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
		////�V�[���؂�ւ�
		//BaseScene* scene = new SelectScene();
		//sceneManager_->SetNextScene(scene);
	}

	//�^�C�g������U������������
	if (--VibCount == 0) {
		input->PadVibrationDef();
	}

	//�����o�֐��|�C���^�Ή������I��
	if (NowSelect == 0) { pFunc = &SelectScene::SelectOper; }
	if (NowSelect == 1) { pFunc = &SelectScene::SelectStart; }
	if (NowSelect == 2) { pFunc = &SelectScene::SelectTitle; }

	//�����o�֐��|�C���^�Ăяo��
	(this->*pFunc)();

	DrawUI();

	//�X�v���C�g�X�V
	sp_stageselect->Update();
	sp_SSoper->Update();
	sp_SSstart->Update();
	sp_SStitle->Update();
	sp_SSNow->Update();
}

void SelectScene::Draw()
{
	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(SspriteBase, dxBase->GetCmdList());
	//// �X�v���C�g�`��
	sp_stageselect->Draw();
	sp_SSoper->Draw();
	sp_SSstart->Draw();
	sp_SStitle->Draw();
	sp_SSNow->Draw();
	if (OperWindOpenFlag == true) { sp_operation_wind->Draw(); }
}

void SelectScene::DrawUI()
{
}
