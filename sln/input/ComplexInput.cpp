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
	else { false; }

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
	else { false; }

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
	else { false; }

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
	else { false; }

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
	else { false; }

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
	else { false; }
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
	else { false; }
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
	else { false; }
}

bool ComplexInput::DecisionByEnter()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	//�p�b�h
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	if (TriggerEnter || PadTriggerA) {
		return true;
	}
	else { false; }
}

bool ComplexInput::PauseOpenClose()
{
	Input* input = Input::GetInstance();

	//�L�[�����Ă����
	const bool Trigger0 = input->TriggerKey(DIK_0);
	//�p�b�h
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));
	if (Trigger0 || PadTriggerStart) {
		return true;
	}
	else { false; }

}
