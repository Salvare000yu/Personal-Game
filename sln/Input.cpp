#include "Input.h"
//#include <wrl.h>

//#define DIRECTINPUT_VERSION 0x0800//directinputバージョン指定
//#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//using namespace Microsoft::WRL;

Input::Input()
{
    Initialize(WinApp::GetInstance());
}

Input::~Input()
{
}

//左スティックの操作を十字キーと連動させる
WORD Input::LeftStickToCross(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
    // 左スティックから十字ボタン変換用
    WORD wButtons = 0;
    
    //左スティックを左に倒す入力を十字ボタン左と連動
    if (sThumbX <= -sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
    }
    //左スティックを右に倒す入力を十字ボタン右と連動
    else if (sThumbX >= sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
    }
    //左スティックを上に倒す入力を十字ボタン上と連動
    if (sThumbY >= sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_UP;
    }
    //左スティックを下に倒す入力を十字ボタン下と連動
    else if (sThumbY <= -sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
    }

    return wButtons;
}



Input* Input::GetInstance()
{
    static Input instance;
    return &instance;
}

void Input::Initialize(WinApp* winApp)
{

    //借りてきたWinAppのインスタンスを記録
    this->winApp = winApp;

    HRESULT result;
    //BYTE key[256] = {};

    //ComPtr<IDirectInput8> dinput = nullptr;
    // 
    //directinputインスタンス生成
    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);//hInstanceの前にw.
    // ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
     //キーボードのデバイス生成
    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
    //入力データ形式セット
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    //排他制御レベルセット
    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

}

void Input::Update()
{
    HRESULT result;

    //前回のキー入力保存
    memcpy(keyPre, key, sizeof(key));

    //キーボード情報取得開始
    result = devkeyboard->Acquire();
    //全キー入力情報取得
    result = devkeyboard->GetDeviceState(sizeof(key), key);
}

//押している間
bool Input::PushKey(BYTE keyNumber)
{
    //指定キーを押していればTRUE
    if (key[keyNumber]) {
        return true;
    }
    //そうでなければ返す
    return false;
}

//押したときのみ
bool Input::TriggerKey(BYTE keyNumber)
{

    //トリガー処理
    // ---前回0,今回0でなければトリガー---
    if (!keyPre[keyNumber] && key[keyNumber]) {
        return true;
    }
    return false;
}

void Input::ControllerInitialize(DWORD controllerNum)
{
    // 構造体のクリア
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    dwResult = XInputGetState(controllerNum, &state);

    // 接続成功
    if (dwResult == ERROR_SUCCESS) {
        return;
    }
    // 接続失敗
    else {
        int a = 0;
        return;
    }
}

void Input::ControllerUpdate(DWORD controllerNum)
{
    // 構造体のクリア
    oldstate = state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    dwResult = XInputGetState(controllerNum, &state);

    if ((state.Gamepad.sThumbLX<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
        state.Gamepad.sThumbLX>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (state.Gamepad.sThumbLY<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {
        state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;
    }

    // 左スティックからの入力を方向パッドに変換
    state.Gamepad.wButtons |= LeftStickToCross(
        state.Gamepad.sThumbLX,
        state.Gamepad.sThumbLY,
        XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
}

bool Input::PushButton(int button)
{
    if (state.Gamepad.wButtons & button) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::PushLt()
{
    if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::PushRt()
{
    if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::PushLeftStickUp()
{
    if (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::PushLeftStickDown()
{
    if (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::PushLeftStickLeft()
{
    if (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::PushLeftStickRight()
{
    if (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
        return true;
    }
    else {
        return false;
    }
}

bool Input::TriggerButton(int button)
{
    if ((state.Gamepad.wButtons & button) && !(oldstate.Gamepad.wButtons & button)) {
        return true;
    }
    else {
        return false;
    }
}
