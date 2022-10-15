#pragma once

#include "BaseScene.h"
/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager
{
public:
	~SceneManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//���V�[���\��
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
private:
	//now�V�[��
	BaseScene* scene_ = nullptr;
	//next
	BaseScene* nextScene_ = nullptr;
};

