#include "Pause.h"
#include "Object3d.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Input.h"
#include "ComplexInput.h"
#include "GameSound.h"
#include "DebugText.h"
#include <DirectXMath.h>

using namespace DirectX;

Pause* Pause::GetInstance()
{
	static Pause instance;
	return &instance;
}

void Pause::Initialize()
{
	//window�c���擾�������Ƃ��g��
	WinApp* winApp = WinApp::GetInstance();

	SpriteBase::GetInstance()->LoadTexture(7, L"Resources/OpenPause.png");
	SpriteBase::GetInstance()->LoadTexture(8, L"Resources/pause.png");
	SpriteBase::GetInstance()->LoadTexture(9, L"Resources/Operation.png");
	SpriteBase::GetInstance()->LoadTexture(10, L"Resources/operation_wind.png");
	SpriteBase::GetInstance()->LoadTexture(11, L"Resources/continuation.png");
	SpriteBase::GetInstance()->LoadTexture(12, L"Resources/GoTitle.png");

	sp_openpause.reset(Sprite::Create(7, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_pause.reset(Sprite::Create(8, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation.reset(Sprite::Create(9, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_operation_wind.reset(Sprite::Create(10, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_continuation.reset(Sprite::Create(11, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_gotitle.reset(Sprite::Create(12, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//�X�v���C�g�|�W�V����
	float gotitlePosY = winApp->window_width / 2;
	sp_openpause->SetPosition({ 1050,600,0 });
	sp_continuation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 450,0 });//��
	sp_operation->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 300,0 });//�^��
	sp_gotitle->SetPosition({ winApp->window_width / 2 - 150,gotitlePosY - 150 ,0 });//��

	//�X�v���C�g�T�C�Y
	sp_openpause->SetSize({ 210.f,130.f });
	sp_continuation->SetSize({ 300.f,100.f });
	sp_gotitle->SetSize({ 300.f,100.f });
	sp_operation->SetSize({ 300.f,100.f });

	sp_openpause->TransferVertexBuffer();
	sp_continuation->TransferVertexBuffer();
	sp_gotitle->TransferVertexBuffer();
	sp_operation->TransferVertexBuffer();

	//���񃊃Z�b�g
	SceneChangeTitleFlag = false;
	PauseFlag = false;
	PauseNowSelect = 0;
}

void Pause::PauseNow()
{

	ComplexInput* cInput = ComplexInput::GetInstance();
	Input* input = Input::GetInstance();
	input->PadVibrationDef();

	/*
	////�I�𒆕\���@�f�o�b�O�p
	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", PauseNowSelect);
		DebugText::GetInstance()->Print(tmp, 430, 460, 5);
	}*/

	//�����o�֐��|�C���^�Ή������I��
	if (PauseNowSelect == 0) { pFunc = &Pause::PauseConti; }
	if (PauseNowSelect == 1) { pFunc = &Pause::PauseOper; }
	if (PauseNowSelect == 2) { pFunc = &Pause::PauseGoTitle; }

	//�����o�֐��|�C���^�Ăяo��
	(this->*pFunc)();

	//����
	WaitKey0++;//�����{�^���łƂ����悤��
	//���������ʌ��Ă�Ƃ��͉����Ă����Ȃ�
	if ((cInput->PauseOpenClose() && WaitKey0 >= 2) && OperWindOpenFlag == false) {
		PauseFlag = false;
		WaitKey0 = 0;
	}

}
void Pause::Update()
{
}
void Pause::Draw()
{
}
void Pause::DrawUI()
{
}
void Pause::Finalize()
{
}
void Pause::PauseConti()
{
	ComplexInput* cInput = ComplexInput::GetInstance();

	//�I�𒆃T�C�Y�ł�����
	sp_continuation->SetSize({ PauseSelectSize,100.f });
	sp_continuation->TransferVertexBuffer();

	if (cInput->tDownArrow()||cInput->tDownMove()) {//1�����͑I��
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 1;
	}
	if (cInput->tUpArrow() || cInput->tUpMove()) {//���2
		sp_continuation->SetSize({ PauseSelectSizeDef,100.f });
		sp_continuation->TransferVertexBuffer();
		PauseNowSelect = 2;
	}

	//�p��
	if (cInput->DecisionByEnter())
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		PauseFlag = false;
	}
}
void Pause::PauseOper()
{
	ComplexInput* cInput = ComplexInput::GetInstance();

	//�I�𒆃T�C�Y�ł�����
	sp_operation->SetSize({ PauseSelectSize,100.f });
	sp_operation->TransferVertexBuffer();

	//��������J���ĂȂ��Ƃ��̂�
	if (OperWindOpenFlag == false)
	{
		if (cInput->tDownArrow() || cInput->tDownMove()) {//����2
			sp_operation->SetSize({ PauseSelectSizeDef,100.f });
			sp_operation->TransferVertexBuffer();
			PauseNowSelect = 2;
		}
		if (cInput->tUpArrow() || cInput->tUpMove()) {//���0
			sp_operation->SetSize({ PauseSelectSizeDef,100.f });
			sp_operation->TransferVertexBuffer();
			PauseNowSelect = 0;
		}
	}
	//���������ʊJ��
	if (cInput->DecisionByEnter())
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		OperWindOpenFlag = true;
		OperationWind();

		WaitKeyEnter++;
		if ((cInput->DecisionByEnter()) && WaitKeyEnter >= 2) {
			OperWindOpenFlag = false;
			WaitKeyEnter = 0;
		}
	}
}
void Pause::OperationWind()
{
	sp_operation_wind->Update();
}
void Pause::PauseGoTitle()
{

	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	//�I�𒆃T�C�Y�ł�����
	sp_gotitle->SetSize({ PauseSelectSize,100.f });
	sp_gotitle->TransferVertexBuffer();
	if (cInput->tDownArrow() || cInput->tDownMove()) {//����0
		sp_gotitle->SetSize({ PauseSelectSizeDef,100.f });
		sp_gotitle->TransferVertexBuffer();
		PauseNowSelect = 0;
	}
	if (cInput->tUpArrow() || cInput->tUpMove()) {//���1
		sp_gotitle->SetSize({ PauseSelectSizeDef,100.f });
		sp_gotitle->TransferVertexBuffer();
		PauseNowSelect = 1;
	}

	//�^�C�g���֖߂�
	if ((cInput->DecisionByEnter()))
	{
		SceneChangeTitleFlag = true;
	}
}

void Pause::SpUpdate()
{
	sp_openpause->Update();
	sp_pause->Update();
	sp_continuation->Update();
	sp_gotitle->Update();
	sp_operation->Update();
}

void Pause::SpOpenPauseDraw()
{
	sp_openpause->Draw();
}

void Pause::SpFlagTrueNowDraw()
{
	sp_pause->Draw();
	sp_continuation->Draw();
	sp_gotitle->Draw();
	sp_operation->Draw();
}

void Pause::SpOperWindDraw()
{
	sp_operation_wind->Draw();
}