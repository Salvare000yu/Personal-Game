#pragma once
/// <summary>
/// �^�C�g�����
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include <memory>
#include "DxBase.h"

class TitleScene :public BaseScene
{

public:

	void Initialize() override;

	void Finalize() override;

	void BeforeUpdate();

	void SceneChange();

	void UpDown();

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;

	bool MoveStartFlag = true;
	float sp = 1.5;
	float StartSp=1.5;

	bool SceneChangeFlag = false;//�V�[���`�F���W�J�n�O

	////---�^�C�g���펞����
	//enum class UpDownPattern {
	//	def,
	//	Up,
	//	Down,
	//};
	//UpDownPattern upDownPattern_ = UpDownPattern::def;

	float time;
	float frame = 0;

private:

	std::unique_ptr < Sprite> sprite1 = nullptr;
	std::unique_ptr < Sprite> sp_gametitlename = nullptr;
	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//----------3dobj
	std::unique_ptr < Model> mod_tunnel = nullptr;//�g���l��
	std::unique_ptr < Model> mod_ground = nullptr;//�n��

	std::unique_ptr < Object3d> obj_tunnel = nullptr;
	std::unique_ptr < Object3d> obj_ground = nullptr;

	int VibCount = 15;//�^�C�g�����牽�t���[���U�������邩
	int SceneChangeVibCount = 15;//�V�[���`�F���W�����t���[���U�������邩

	float NamePosXCenter = 0;
	float NamePosYCenter = 0;

	////���꒴������p�^�[���ς���@�ő�ŏ�
	//const float NamePosMoveMax = 7;
	//const float NamePosMoveMin = -7;
	////�グ��������l
	//const float NamePosYUpDownDef = 0.2;//�f�t�H
	//float NamePosYUpDown = NamePosYUpDownDef;
};

