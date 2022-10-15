#pragma once

class SceneManager;

/// <summary>
/// �V�[�����
/// </summary>
class BaseScene
{
public:
    BaseScene();

    virtual ~BaseScene() = default;

    virtual void Initialize()=0;

    virtual void Finalize()=0;

    virtual void Update()=0;

    virtual void Draw()=0;
    virtual void DrawUI()=0;

    virtual void SetSceneManager(SceneManager* sceneManager) {
        sceneManager_ = sceneManager; }
protected:
    //�V�[���}�l�[�W����
    SceneManager* sceneManager_ = nullptr;

};

