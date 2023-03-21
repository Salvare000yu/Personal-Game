#pragma once
#include "Sprite.h"
#include "BaseObject.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include "DxBase.h"
using namespace DirectX;

/// <summary>
/// �V�[���J�ڎ��̉��o
/// </summary>

class SceneChangeDirection
{
public:
	static SceneChangeDirection* GetInstance();
	void Initialize();

	void HideTheScreen();

	void OpenTheScreen();

	void Update();

	void Draw();

	//�V�[���J�ڃt���O�@false:���ĂȂ�
	bool SceneChangeDirectionFlag;
	//�V�[���J�ڊ����@false:�����O
	bool SceneChangeCompFlag;
	//��ʉB���@false:�B���n�߂�O
	bool HideTheScreenFlag;
	//playscene���n�܂艉�o�n�߂�@false:�܂�
	bool GameReadyStartFlag;

private:

	std::unique_ptr < Sprite> sp_scenechange = nullptr;

	const float HideSp = 40;//��ʉB�����x �摜�T�C�Y�Ŋ����傫��

	float HideVel = 0;//���ۂɍ��W�ɑ����l

};