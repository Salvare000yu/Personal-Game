#include <Windows.h>

#pragma once
class WinApp
{
	WinApp(const WinApp& winApp) = delete;
	WinApp& operator=(const WinApp& winApp) = delete;

	WinApp() {};

public://�ÓI�����o�֐�
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://�����o�֐�
	static WinApp* GetInstance();

	//������
	void Initialize();
	//�X�V
	void Update();

	//�I��
	void Finalize();

	bool ProcessMessage();

	//---------------getter-------------//
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() { return w.hInstance; }

public://�萔
	//�E�B���h�E����
	static const int window_width = 1280;
	//�E�B���h�E�c��
	static const int window_height = 720;

private:
	//�E�B���h�E�n���h��
	HWND hwnd = nullptr;
	//�E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
};

