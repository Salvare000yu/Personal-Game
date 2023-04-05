#include "ComplexInput.h"

ComplexInput* ComplexInput::GetInstance()
{
	static ComplexInput instance;
	return &instance;
}

bool ComplexInput::LeftMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool inputA = input->PushKey(DIK_A);
	//パッド
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	if (inputA || PadInputLEFT) {
		return true;
	}

	return false;
}

bool ComplexInput::RightMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool inputD = input->PushKey(DIK_D);
	//パッド
	const bool PadInputRight = input->PushButton(static_cast<int>(Button::RIGHT));
	if (inputD || PadInputRight) {
		return true;
	}

	return false;
}

bool ComplexInput::UpMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool inputW = input->PushKey(DIK_W);
	//パッド
	const bool PadInputUp = input->PushButton(static_cast<int>(Button::UP));
	if (inputW || PadInputUp) {
		return true;
	}

	return false;
}

bool ComplexInput::DownMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool inputS = input->PushKey(DIK_S);
	//パッド
	const bool PadInputDown = input->PushButton(static_cast<int>(Button::DOWN));
	if (inputS || PadInputDown) {
		return true;
	}

	return false;
}
//trigger↓
bool ComplexInput::tLeftMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool triggerA = input->TriggerKey(DIK_A);
	//パッド
	const bool PadTriggerLEFT = input->TriggerButton(static_cast<int>(Button::LEFT));
	if (triggerA || PadTriggerLEFT) {
		return true;
	}

	return false;
}

bool ComplexInput::tRightMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool triggerD = input->TriggerKey(DIK_D);
	//パッド
	const bool PadTriggerRight = input->TriggerButton(static_cast<int>(Button::RIGHT));
	if (triggerD || PadTriggerRight) {
		return true;
	}

	return false;
}

bool ComplexInput::tUpMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool triggerW = input->TriggerKey(DIK_W);
	//パッド
	const bool PadTriggerUp = input->TriggerButton(static_cast<int>(Button::UP));
	if (triggerW || PadTriggerUp) {
		return true;
	}

	return false;
}

bool ComplexInput::tDownMove()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool triggerS = input->TriggerKey(DIK_S);
	//パッド
	const bool PadTriggerDown = input->TriggerButton(static_cast<int>(Button::DOWN));
	if (triggerS || PadTriggerDown) {
		return true;
	}

	return false;
}

bool ComplexInput::tUpArrow()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerUp = input->TriggerKey(DIK_UP);
	//パッド
	const bool PadInputUp = input->TriggerButton(static_cast<int>(Button::UP));
	if (TriggerUp || PadInputUp) {
		return true;
	}

	return false;
}
bool ComplexInput::tDownArrow()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerDown = input->TriggerKey(DIK_DOWN);
	//パッド
	const bool PadInputDown = input->TriggerButton(static_cast<int>(Button::DOWN));
	if (TriggerDown || PadInputDown) {
		return true;
	}

	return false;
}
bool ComplexInput::tRightArrow()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerRight = input->TriggerKey(DIK_RIGHT);
	//パッド
	const bool PadInputRight = input->TriggerButton(static_cast<int>(Button::RIGHT));
	if (TriggerRight || PadInputRight) {
		return true;
	}

	return false;
}
bool ComplexInput::tLeftArrow()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerLeft = input->TriggerKey(DIK_LEFT);
	//パッド
	const bool PadInputLeft = input->TriggerButton(static_cast<int>(Button::LEFT));
	if (TriggerLeft || PadInputLeft) {
		return true;
	}

	return false;
}

bool ComplexInput::Decision()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//パッド
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));
	if (TriggerEnter || PadTriggerA || TriggerSPACE) {
		return true;
	}

	return false;
}

bool ComplexInput::PauseOpenClose()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerP = input->TriggerKey(DIK_P);
	//パッド
	const bool PadTriggerStart = input->TriggerButton(static_cast<int>(Button::START));
	if (TriggerP || PadTriggerStart) {
		return true;
	}

	return false;
}

bool ComplexInput::PlayerDash()
{
	Input* input = Input::GetInstance();

	//キー押している間
	const bool TriggerMouseRight = input->TriggerMouse(1);
	const bool TrigerLShift = input->TriggerKey(DIK_LSHIFT);
	const bool TrigerRShift = input->TriggerKey(DIK_RSHIFT);
	//パッド
	const bool PadInputX = input->TriggerButton(static_cast<int>(Button::X));

	if (TriggerMouseRight || TrigerLShift || TrigerRShift || PadInputX) {
		return true;
	}

	return false;
}