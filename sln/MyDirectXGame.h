#pragma once

#include "GameBase.h"

#include "FbxLoader.h"

#include "BaseScene.h"

#include <vector>

/// <summary>
/// �Q�[���ŗL�N���X
/// </summary>
class MyDirectXGame:public GameBase
{
public:

    void Initialize() override;

    void Finalize() override;

    /// <summary>
    /// ���t���[���X�V
    /// </summary>
    void Update() override;
    /// <summary>
    /// �`��
    /// </summary>
    void Draw() override;

private:
    ////�V�[��
    //InterfaceScene* scene_ = nullptr;
    //����������
};

