#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800//directinputバージョン指定
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

	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバ関数

	static Input* GetInstance();

	//更新
	void Update();

	/// <summary>
	/// キーの押下チェック
	/// </summary>
	/// 	キー番号
	/// 	押されているか
	bool PushKey(BYTE keyNumber);

	/// <summary>
/// キーのトリガー
/// </summary>
/// 	キー番号
/// 	トリガーか
	bool TriggerKey(BYTE keyNumber);

	// コントローラー初期化
	void ControllerInitialize(DWORD controllerNum);
	// コントローラー更新
	void ControllerUpdate(DWORD controllerNum);

	//--------------------Xinput↓
	XINPUT_STATE state;
	XINPUT_STATE oldstate;
	DWORD dwResult;
	// スティックの入力を十字ボタンに変換
	WORD ThumbToPad(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone);

	/// <summary>
	/// コントローラーボタンの押し下をチェック
	/// </summary>
	/// <param name="button">ボタン</param>
	/// <returns>押されているか</returns>
	bool PushButton(int button);

	/// <summary>
	/// コントローラーボタンの押し下をチェック
	/// </summary>
	/// <returns>左トリガーか</returns>
	bool PushLt();

	/// <summary>
	/// コントローラーボタンの押し下をチェック
	/// </summary>
	/// <returns>右トリガーか</returns>
	bool PushRt();

	/// <summary>
	/// コントローラースティックの押し下をチェック
	/// </summary>
	/// <returns>左スティック上が押されているか</returns>
	bool PushLeftStickUp();

	/// <summary>
	/// コントローラースティックの押し下をチェック
	/// </summary>
	/// <returns>左スティック下が押されているか</returns>
	bool PushLeftStickDown();

	/// <summary>
	/// コントローラースティックの押し下をチェック
	/// </summary>
	/// <returns>左スティック左が押されているか</returns>
	bool PushLeftStickLeft();

	/// <summary>
	/// コントローラースティックの押し下をチェック
	/// </summary>
	/// <returns>左スティック右が押されているか</returns>
	bool PushLeftStickRight();

	/// <summary>
	/// コントローラーボタンの押し下をチェック
	/// </summary>
	/// <param name="button">ボタン</param>
	/// <returns>トリガーボタンか</returns>
	bool TriggerButton(int button);
	//--------------------Xinput↑


private://メンバ変数
	//初期化
	void Initialize(WinApp* WinApp);

	Input(const Input& input) = delete;
	Input& operator=(const Input& input) = delete;

	Input();
	~Input();

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;

	//全キーの配置
	BYTE key[256] = {};

	//前回の全キーの配置
	BYTE keyPre[256] = {};

	//directinputのインスタンス
	ComPtr<IDirectInput8>dinput;

	//WindowsAPI
	WinApp* winApp = nullptr;


};

