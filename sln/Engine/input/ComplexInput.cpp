#include "ComplexInput.h"

ComplexInput* ComplexInput::GetInstance()
{
	static ComplexInput instance;
	return &instance;
}

bool ComplexInput::LeftMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool inputA = input->PushKey(DIK_A);
	//�p�b�h
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	if (inputA || PadInputLEFT) {
		return true;
	}

	return false;
}

bool ComplexInput::RightMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool inputD = input->PushKey(DIK_D);
	//�p�b�h
	const bool PadInputRight = input->PushButton(static_cast<int>(Button::RIGHT));
	if (inputD || PadInputRight) {
		return true;
	}

	return false;
}

bool ComplexInput::UpMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool inputW = input->PushKey(DIK_W);
	//�p�b�h
	const bool PadInputUp = input->PushButton(static_cast<int>(Button::UP));
	if (inputW || PadInputUp) {
		return true;
	}

	return false;
}

bool ComplexInput::DownMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool inputS = input->PushKey(DIK_S);
	//�p�b�h
	const bool PadInputDown = input->PushButton(static_cast<int>(Button::DOWN));
	if (inputS || PadInputDown) {
		return true;
	}

	return false;
}
//trigger��
bool ComplexInput::tLeftMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool triggerA = input->TriggerKey(DIK_A);
	//�p�b�h
	const bool PadTriggerLEFT = input->TriggerButton(static_cast<int>(Button::LEFT));
	if (triggerA || PadTriggerLEFT) {
		return true;
	}

	return false;
}

bool ComplexInput::tRightMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool triggerD = input->TriggerKey(DIK_D);
	//�p�b�h
	const bool PadTriggerRight = input->TriggerButton(static_cast<int>(Button::RIGHT));
	if (triggerD || PadTriggerRight) {
		return true;
	}

	return false;
}

bool ComplexInput::tUpMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool triggerW = input->TriggerKey(DIK_W);
	//�p�b�h
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	if (triggerW || PadTriggerUp) {
		return true;
	}

	return false;
}

bool ComplexInput::tDownMove()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool triggerS = input->TriggerKey(DIK_S);
	//�p�b�h
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));
	if (triggerS || PadTriggerDown) {
		return true;
	}

	return false;
}

bool ComplexInput::tUpArrow()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	//�p�b�h
	const bool PadInputUp = input->TriggerButton(static_cast<int>(Button::UP));
	if (TriggerUp || PadInputUp) {
		return true;
	}

	return false;
}
bool ComplexInput::tDownArrow()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	//�p�b�h
	const bool PadInputDown = input->TriggerButton(static_cast<int>(Button::DOWN));
	if (TriggerDown || PadInputDown) {
		return true;
	}

	return false;
}
bool ComplexInput::tRightArrow()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerRight = input->TriggerKey(DIK_RIGHT);
	//�p�b�h
	const bool PadInputRight = input->TriggerButton(static_cast<int>(Button::RIGHT));
	if (TriggerRight || PadInputRight) {
		return true;
	}

	return false;
}
bool ComplexInput::tLeftArrow()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerLeft = input->TriggerKey(DIK_LEFT);
	//�p�b�h
	const bool PadInputLeft = input->TriggerButton(static_cast<int>(Button::LEFT));
	if (TriggerLeft || PadInputLeft) {
		return true;
	}

	return false;
}

bool ComplexInput::Decision()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//�p�b�h
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	if (TriggerEnter || PadTriggerA || TriggerSPACE) {
		return true;
	}

	return false;
}

bool ComplexInput::PauseOpenClose()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerP = input->TriggerKey(DIK_P);
	//�p�b�h
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));
	if (TriggerP || PadTriggerStart) {
		return true;
	}

	return false;
}

bool ComplexInput::PlayerDash()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerMouseRight = input->TriggerMouse(1);
	const bool TrigerLShift = input->TriggerKey(DIK_LSHIFT);
	const bool TrigerRShift = input->TriggerKey(DIK_RSHIFT);
	//�p�b�h
	const bool PadInputX = input->TriggerButton(static_cast<int>(Button::X));

	if (TriggerMouseRight || TrigerLShift || TrigerRShift || PadInputX) {
		return true;
	}

	return false;
}