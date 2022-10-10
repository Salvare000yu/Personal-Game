#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800//directinput�o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
#include <Xinput.h>

#pragma once

#pragma comment(lib,"xinput.lib")

enum class Button {
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	UP = XINPUT_GAMEPAD_DPAD_UP,
	DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	LEFTTHUMB = XINPUT_GAMEPAD_LEFT_THUMB,
	RIGHTTHUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
};


class Input
{
public:

	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����o�֐�

	static Input* GetInstance();

	//�X�V
	void Update();

	/// <summary>
	/// �L�[�̉����`�F�b�N
	/// </summary>
	/// 	�L�[�ԍ�
	/// 	������Ă��邩
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[
	/// </summary>
	/// 	�L�[�ԍ�
	/// 	�g���K�[��
	bool TriggerKey(BYTE keyNumber);

	//--------------------Xinput��
	// 
	// �R���g���[���[������
	void ControllerInitialize(DWORD controllerNum);
	// �R���g���[���[�X�V
	void ControllerUpdate(DWORD controllerNum);

	XINPUT_STATE state;
	XINPUT_STATE oldstate;
	DWORD dwResult;
	//���X�e�B�b�N�̓��͂��\���{�^���ɕϊ�
	WORD LeftStickToCross(SHORT LeftStickX, SHORT LeftStickY, SHORT sDeadZone);

	//�{�^�������Ă����
	bool PushButton(int button);

	/// <summary>
	/// �R���g���[���[�{�^���̉��������`�F�b�N
	/// </summary>
	/// <returns>���g���K�[��</returns>
	bool PushLt();

	/// <summary>
	/// �R���g���[���[�{�^���̉��������`�F�b�N
	/// </summary>
	/// <returns>�E�g���K�[��</returns>
	bool PushRt();

	//���X�e�B�b�N�����
	bool PushLeftStickUp();

	//���X�e�B�b�N������
	bool PushLeftStickDown();

	//���X�e�B�b�N������
	bool PushLeftStickLeft();

	//���X�e�B�b�N�E����
	bool PushLeftStickRight();

	//�{�^���������u�Ԃ̂�
	bool TriggerButton(int button);
	//--------------------Xinput��

	inline const POINT& GetMousePos() const { return mousePos; };
	bool SetMousePos(int x, int y);

	enum MouseButton
	{
		LEFT = 0,
		RIGHT = 1
	};

	/// <param name="mouseNumber">0�ō��N���b�N,1�ŉE�N���b�N</param>
	inline bool PushMouse(_In_ BYTE mouseNumber)
	{
		return (bool)mouse.rgbButtons[mouseNumber];
	}

	/// <param name="mouseNumber">0�ō��N���b�N,1�ŉE�N���b�N</param>
	inline bool TriggerMouse(_In_ BYTE mouseNumber)
	{
		return PushMouse(mouseNumber) && !(bool)mousePre.rgbButtons[mouseNumber];
	}

private://�����o�ϐ�
	//������
	void Initialize(WinApp* WinApp);

	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;

	Input();
	~Input();

	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;

	//�S�L�[�̔z�u
	BYTE key[256] = {};

	//�O��̑S�L�[�̔z�u
	BYTE keyPre[256] = {};

	//directinput�̃C���X�^���X
	ComPtr<IDirectInput8>dinput;

	//WindowsAPI
	WinApp* winApp = nullptr;

	POINT mousePos;

	DIMOUSESTATE2 mouse{};
	DIMOUSESTATE2 mousePre{};
	ComPtr<IDirectInputDevice8> devmouse;

};

