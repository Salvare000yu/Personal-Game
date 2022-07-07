#pragma once

#include "GameBase.h"

#include "FbxLoader.h"

#include "BaseScene.h"

#include <vector>

/// <summary>
/// ゲーム固有クラス
/// </summary>
class MyDirectXGame:public GameBase
{
public:

    void Initialize() override;

    void Finalize() override;

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void Update() override;
    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:
    ////シーン
    //InterfaceScene* scene_ = nullptr;
    //いつかけす↑
};

