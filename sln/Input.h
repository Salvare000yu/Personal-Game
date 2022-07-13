#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800//directinput�o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"

#pragma once
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

};

