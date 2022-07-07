#include "Input.h"
//#include <wrl.h>

//#define DIRECTINPUT_VERSION 0x0800//directinput�o�[�W�����w��
//#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//using namespace Microsoft::WRL;

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

bool Input::PushKey(BYTE keyNumber)
{
    //�w��L�[�������Ă����TRUE
    if (key[keyNumber]) {
        return true;
    }
    //�����łȂ���ΕԂ�
    return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{

    //�g���K�[����
    // ---�O��0,����0�łȂ���΃g���K�[---
    if (!keyPre[keyNumber] && key[keyNumber]) {
        return true;
    }
    return false;
}