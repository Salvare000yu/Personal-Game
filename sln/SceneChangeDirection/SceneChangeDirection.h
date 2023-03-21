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

	void Update();

	void Draw();

	//�V�[���J�ڃt���O�@false:���ĂȂ�
	bool SceneChangeDirectionFlag = false;
	//�V�[���J�ڊ����@false:�����O
	bool SceneChangeCompFlag = false;
	//��ʉB���@false:�B���O
	bool HideTheScreenFlag = false;

private:

	std::unique_ptr < Sprite> sp_scenechange = nullptr;

	const float HideSp = 10;//��ʉB�����x

	float HideVel = 0;//���ۂɍ��W�ɑ����l

};