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
    /// 実行
    /// </summary>
    void Run();

    /// <summary>
/// 初期化
/// </summary>
    virtual void Initialize();

    /// <summary>
/// 終了
/// </summary>
    virtual void Finalize();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    virtual void Update();

    /// <summary>
/// 描画
/// </summary>
    virtual void Draw();

    virtual bool IsEndReq() { return endReq_; }
protected:
    bool endReq_ = false;
    //ポインタ系ここ
    Input* input = nullptr;
    WinApp* winApp = nullptr;
    DxBase* dxBase = nullptr;
    GameSound* gameSound = nullptr;
    DebugText* debugText = nullptr;
    SpriteBase* spriteBase = nullptr;
    SceneManager* sceneManager_ = nullptr;
    Camera* camera = nullptr;
};

