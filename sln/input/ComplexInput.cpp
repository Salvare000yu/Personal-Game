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
	else { false; }

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
	else { false; }

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
	else { false; }

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
	else { false; }

}