#include "Input.h"
//#include <wrl.h>

//#define DIRECTINPUT_VERSION 0x0800//directinput�o�[�W�����w��
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

//���X�e�B�b�N�̑�����\���L�[�ƘA��������
WORD Input::LeftStickToCross(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
    // ���X�e�B�b�N����\���{�^���ϊ��p
    WORD wButtons = 0;
    
    //���X�e�B�b�N�����ɓ|�����͂��\���{�^�����ƘA��
    if (sThumbX <= -sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
    }
    //���X�e�B�b�N���E�ɓ|�����͂��\���{�^���E�ƘA��
    else if (sThumbX >= sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
    }
    //���X�e�B�b�N����ɓ|�����͂��\���{�^����ƘA��
    if (sThumbY >= sDeadZone) {
        wButtons |= XINPUT_GAMEPAD_DPAD_UP;
    }
    //���X�e�B�b�N�����ɓ|�����͂��\���{�^�����ƘA��
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

    //�؂�Ă���WinApp�̃C���X�^���X���L�^
    this->winApp = winApp;

    HRESULT result;
    //BYTE key[256] = {};

    //ComPtr<IDirectInput8> dinput = nullptr;
    // 
    //directinput�C���X�^���X����
    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);//hInstance�̑O��w.
    // ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
     //�L�[�{�[�h�̃f�o�C�X����
    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
    //���̓f�[�^�`���Z�b�g
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
    //�r�����䃌�x���Z�b�g
    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

}

void Input::Update()
{
    HRESULT result;

    //�O��̃L�[���͕ۑ�
    memcpy(keyPre, key, sizeof(key));

    //�L�[�{�[�h���擾�J�n
    result = devkeyboard->Acquire();
    //�S�L�[���͏��擾
    result = devkeyboard->GetDeviceState(sizeof(key), key);
}

//�����Ă����
bool Input::PushKey(BYTE keyNumber)
{
    //�w��L�[�������Ă����TRUE
    if (key[keyNumber]) {
        return true;
    }
    //�����łȂ���ΕԂ�
    return false;
}

//�������Ƃ��̂�
bool Input::TriggerKey(BYTE keyNumber)
{

    //�g���K�[����
    // ---�O��0,����0�łȂ���΃g���K�[---
    if (!keyPre[keyNumber] && key[keyNumber]) {
        return true;
    }
    return false;
}

void Input::ControllerInitialize(DWORD controllerNum)
{
    // �\���̂̃N���A
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    dwResult = XInputGetState(controllerNum, &state);

    // �ڑ�����
    if (dwResult == ERROR_SUCCESS) {
        return;
    }
    // �ڑ����s
    else {
        int a = 0;
        return;
    }
}

void Input::ControllerUpdate(DWORD controllerNum)
{
    // �\���̂̃N���A
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

    // ���X�e�B�b�N����̓��͂�����p�b�h�ɕϊ�
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
