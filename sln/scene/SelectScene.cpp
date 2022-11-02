#include "SelectScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"
#include "WinApp.h"
#include "TitleScene.h"

#include "PostEffect.h"

using namespace DirectX;

void SelectScene::Initialize()
{
	//window�c���擾�������Ƃ��g��
	WinApp* winApp = WinApp::GetInstance();

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("C_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

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

	sp_SSNow->SetPosition({ sp_SSoper->GetPosition().x - 50,sp_SSoper->GetPosition().y - 50,0 });
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

	XMFLOAT3 SSOp0_1pos = sp_SSNow->GetPosition();

	if (WaitKeyEase < 20) { WaitKeyEase++; }//���t������
	//��������J���ĂȂ��Ƃ��̂�
	if (OperWindOpenFlag == false && WaitKeyEase >= 2)//�܂��Ă�����͎�t
	{
		if (TriggerRight || PadTriggerRight) {//1�����͑I��
			selectPattern_ = SelectPattern::operationTOstart;//[�����������X�^�[�g��]�ɕύX
		}
		//if (TriggerLeft || PadTriggerLeft) {
		//	NowSelect = 2;
		//}
	}

	//����������
	if (SSOp0_1Flag == false)
	{
		if (TriggerEnter || PadTriggerA && CursorMoveNowFlag == false)
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			OperWindOpenFlag = true;
			OperationWind();

			WaitKeyEnter++;
			if ((TriggerEnter || PadTriggerA) && WaitKeyEnter >= 2 && CursorMoveNowFlag == false) {
				OperWindOpenFlag = false;
				WaitKeyEnter = 0;
			}
		}
	}

	{//���֐���
		//�C�[�Y�l����ĖڕW���W������\��Ȃ�
		if((SSOp0_1pos.x + (EaseVal - DecEaseVal))> (sp_SSstart->GetPosition().x - 50))
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
			sp_SSNow->SetPosition({ SSOp0_1pos.x,sp_SSoper->GetPosition().y - 50,0 });
		}
	}

	//--------���Z���N�g�p�^�[��enum
	//�����������X�^�[�g��
	if (selectPattern_ == SelectPattern::operationTOstart) {

		selectPattern_ = SelectPattern::def;
		SSOp0_1Flag = true;

	}
	//--------���Z���N�g�p�^�[��enum
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

	XMFLOAT3 SSStartPos = sp_SSNow->GetPosition();

	if (WaitKeyEase < 20) { WaitKeyEase++; }//���t������
	if (WaitKeyEase >= 2 && CursorMoveNowFlag == false)
	{
		if (TriggerRight || PadTriggerRight) {
			if (WaitKeyEase >= 2)//���͑҂�
			{
				selectPattern_ = SelectPattern::startTOtitle;//[�X�^�[�g����^�C�g��]�ɕύX
			}
		}
		if (TriggerLeft || PadTriggerLeft) {
			if (WaitKeyEase >= 2)//���͑҂�
			{
				selectPattern_ = SelectPattern::startTOoperation;//[�X�^�[�g���瑀�������]�ɕύX
			}
		}
	}

	//�v���C�V�[����
	if (SSStart1_2Flag == false&& SSStart1_0Flag==false)
	{
		if (TriggerEnter || PadTriggerA && CursorMoveNowFlag == false)
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();
			// ������~
			GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
			//�V�[���؂�ւ�
			BaseScene* scene = new GamePlayScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	//----------------���W���B�Ŏ��֐���
	{//���֐���
		//�C�[�Y�l����ĖڕW���W������\��Ȃ�
		if ((SSStartPos.x + (EaseVal - DecEaseVal)) > (sp_SStitle->GetPosition().x - 50))
		{
			EaseVal = EaseValDef;//�C�[�Y�l������
			CursorMoveNowFlag = false;//���R�ɂ��Ă悢
			WaitKeyEase = 0;//��t�҂����ɖ߂�
			NowSelect = 2;
			SSStart1_2Flag = false;
		}
	}
	{//���֐���
		//�C�[�Y�l����ĖڕW���W������\��Ȃ�
		if ((SSStartPos.x - (EaseVal - DecEaseVal)) < (sp_SSoper->GetPosition().x - 50))
		{
			EaseVal = EaseValDef;//�C�[�Y�l������
			CursorMoveNowFlag = false;//���R�ɂ��Ă悢
			WaitKeyEase = 0;//��t�҂����ɖ߂�
			NowSelect = 0;
			SSStart1_0Flag = false;
		}
	}
	//----------------���W���B�Ŏ��֐���

	//----------------�ړ�������
	{//�ړ�����
		//1����2�ֈړ�
		if (SSStart1_2Flag == true)
		{
			CursorMoveNowFlag = true;//�����Ă邩����̓_����
			SSStartPos.x += EaseVal;
			EaseVal -= DecEaseVal;
			sp_SSNow->SetPosition({ SSStartPos.x,sp_SSstart->GetPosition().y - 50,0 });
		}
	}
	{//�ړ�����
	//1����0�ֈړ�
		if (SSStart1_0Flag == true)
		{
			CursorMoveNowFlag = true;//�����Ă邩����̓_����
			SSStartPos.x -= EaseVal;
			EaseVal -= DecEaseVal;
			sp_SSNow->SetPosition({ SSStartPos.x,sp_SSstart->GetPosition().y - 50,0 });
		}
	}
	//----------------�ړ�������

	//--------���Z���N�g�p�^�[��enum
	//�X�^�[�g����^�C�g����
	if (selectPattern_ == SelectPattern::startTOtitle) {
		SSStart1_2Flag = true;
		selectPattern_ = SelectPattern::def;

	}
	//�X�^�[�g���瑀�������
	if (selectPattern_ == SelectPattern::startTOoperation) {
		SSStart1_0Flag = true;
		selectPattern_ = SelectPattern::def;

	}
	//--------���Z���N�g�p�^�[��enum
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

	XMFLOAT3 SStitle2_1 = sp_SSNow->GetPosition();

	if (WaitKeyEase < 20) { WaitKeyEase++; }//���t������
	if (WaitKeyEase >= 2 && CursorMoveNowFlag == false)
	{
		//if (TriggerRight || PadTriggerRight) {
		//	NowSelect = 0;
		//}
		if (TriggerLeft || PadTriggerLeft) {
			selectPattern_ = SelectPattern::titleTOstart;//[�^�C�g������X�^�[�g��]�ɕύX
		}
	}

	//�^�C�g���V�[����
	if (SStitle2_1Flag==false)
	{
		if (TriggerEnter || PadTriggerA && CursorMoveNowFlag == false)
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			input->PadVibration();
			// ������~
			GameSound::GetInstance()->SoundStop("C_rhythmaze_125.wav");
			//�V�[���؂�ւ�
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}
	}

	{//���֐���
		//�C�[�Y�l����ĖڕW���W������\��Ȃ�
		if ((SStitle2_1.x - (EaseVal - DecEaseVal)) < (sp_SSstart->GetPosition().x - 50))
		{
			EaseVal = EaseValDef;//�C�[�Y�l������
			CursorMoveNowFlag = false;//���R�ɂ��Ă悢
			WaitKeyEase = 0;//��t�҂����ɖ߂�
			NowSelect = 1;
			SStitle2_1Flag = false;
		}
	}

	{//�ړ�����
		//2����1�ֈړ�
		if (SStitle2_1Flag == true)
		{
			CursorMoveNowFlag = true;//�����Ă邩����̓_����
			SStitle2_1.x -= EaseVal;
			EaseVal -= DecEaseVal;
			sp_SSNow->SetPosition({ SStitle2_1.x,sp_SStitle->GetPosition().y - 50,0 });
		}
	}

	//--------���Z���N�g�p�^�[��enum
	//�����������X�^�[�g��
	if (selectPattern_ == SelectPattern::titleTOstart) {
		SStitle2_1Flag = true;
		selectPattern_ = SelectPattern::def;

	}
	//--------���Z���N�g�p�^�[��enum
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
	////�C�[�W���O�����͑ҋ@���ԓI��
	//char tmp[32]{};
	//sprintf_s(tmp, 32, "%2.f", WaitKeyEase);
	//DebugText::GetInstance()->Print(tmp, 430, 430, 3);
}
