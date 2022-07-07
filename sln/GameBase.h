#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DxBase.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"
#include "DebugText.h"
#include "GameSound.h"
#include "SceneManager.h"
#include "FbxLoader.h"

#include "Camera.h"

class GameBase
{
public:

    /// <summary>
    /// ���s
    /// </summary>
    void Run();

    /// <summary>
/// ������
/// </summary>
    virtual void Initialize();

    /// <summary>
/// �I��
/// </summary>
    virtual void Finalize();

    /// <summary>
    /// ���t���[���X�V
    /// </summary>
    virtual void Update();

    /// <summary>
/// �`��
/// </summary>
    virtual void Draw();

    virtual bool IsEndReq() { return endReq_; }
protected:
    bool endReq_ = false;
    //�|�C���^�n����
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    DxBase* dxBase = nullptr;
    GameSound* gameSound = nullptr;
    DebugText* debugText = nullptr;
    SpriteBase* spriteBase = nullptr;
    SceneManager* sceneManager_ = nullptr;
    Camera* camera = nullptr;
};

