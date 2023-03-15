#pragma once
/// <summary>
/// �^�C�g�����
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "BaseObject.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include "Player.h"
#include <memory>
#include "DxBase.h"
#include "CameraTracking.h"
#include "Camera.h"

using namespace DirectX;

class TitleScene :public BaseScene, public BaseObject
{

public:

	void Initialize() override;

	void Finalize() override;

	void BeforeUpdate();

	void PlayerAppear();//���@�̓o��

	void SceneChange();

	void UpDown();

	void DoorOpen();//�����J��

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;

	bool PAppearFlag = true;//���@�o��t���O�@true�F�o�ꒆ
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

	std::unique_ptr < Player> player_ = nullptr;

	float time;
	float frame = 0;

private:

	std::unique_ptr < Sprite> sprite1 = nullptr;
	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//----------3dobj
	std::unique_ptr < Model> mod_tunnel = nullptr;//�g���l��
	std::unique_ptr < Model> mod_ground = nullptr;//�n��
	std::unique_ptr < Model> mod_player = nullptr;// ���@
	std::unique_ptr < Model> mod_kaberight = nullptr;//��
	std::unique_ptr < Model> mod_kabeleft = nullptr;//��
	std::unique_ptr < Model> mod_logo = nullptr;//�^�C�g�����S

	std::unique_ptr < Object3d> obj_tunnel = nullptr;
	std::unique_ptr < Object3d> obj_ground = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;
	std::unique_ptr < Object3d> obj_logo = nullptr;//�^�C�g�����S

	//----------�o��ޏꋤ��----------//
	const float PApMoveFrameMax = 60.f;//�o�ߎ��ԍő�t���[��(�o��)
	const float PExitMoveFrameMax = 100.f;//�o�ߎ��ԍő�t���[��(�ޏ�)
	const int PMoveFrameDef = 0;//�o�ߎ��Ԃłӂ�
	int PMoveFrame = PMoveFrameDef;//�o�ߎ���
	//-----���@�o��
	XMFLOAT3 ApStartPPos;//�J�n�����@���W
	XMFLOAT3 ApEndPPos;//�I��
	const float CamEyeMoveSpX = 0.7f;//�J���������炷�l
	//------���@�o��
	//------���@�ޏ�(�V�[���`�F���W)
	XMFLOAT3 ExitStartPPos;//�J�n�����@���W
	XMFLOAT3 ExitEndPPos;//�I��
	const float ExitPosZ = 0;//�����ł����
	//------���@�ޏ�(�V�[���`�F���W)

	int VibCount = 15;//�^�C�g�����牽�t���[���U�������邩
	int SceneChangeVibCount = 15;//�V�[���`�F���W�����t���[���U�������邩

	float NamePosXCenter = 0;
	float NamePosYCenter = 0;

	bool DoorOpenFlag = false;//���J���ĂȂ�

	// �J����
	std::unique_ptr<CameraTracking> camera;

	////���꒴������p�^�[���ς���@�ő�ŏ�
	//const float NamePosMoveMax = 7;
	//const float NamePosMoveMin = -7;
	////�グ��������l
	//const float NamePosYUpDownDef = 0.2;//�f�t�H
	//float NamePosYUpDown = NamePosYUpDownDef;
};

