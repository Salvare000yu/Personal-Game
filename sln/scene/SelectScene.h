#pragma once
/// <summary>
/// �Z���N�g���
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include <memory>
#include "DxBase.h"

class SelectScene :public BaseScene
{
public:

	enum class SelectPattern {
		def,
		operationTOstart,
		operationTOtitle,
		startTOtitle,
		startTOoperation,
		titleTOoperation,
		titleTOstart,
	};

	SelectPattern selectPattern_ = SelectPattern::def;

	void Initialize() override;

	void SelectOper();

	void OperationWind();

	void SelectStart();

	void SelectTitle();

	void Finalize() override;

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;
private:

	std::unique_ptr < Sprite> sp_stageselect = nullptr;
	std::unique_ptr < Sprite> sp_SSoper = nullptr;
	std::unique_ptr < Sprite> sp_SSstart = nullptr;
	std::unique_ptr < Sprite> sp_SStitle = nullptr;
	std::unique_ptr < Sprite> sp_SSNow = nullptr;
	std::unique_ptr < Sprite> sp_operation_wind = nullptr;

	//Model* model_1 = nullptr;

	//Object3d* object3d_1 = nullptr;

	bool VibFlag = false;

	int VibCount = 15;//�^�C�g�����牽�t���[���U�������邩

	float NowSelect = 0;//0:��������i�f�t�H�j 1:�X�^�[�g�@2:�^�C�g����
	void  (SelectScene::* pFunc)();

	int WaitKeyEnter = 0;//Enter�ő���������߂�..
	bool OperWindOpenFlag = false;//�J���Ă��Ȃ�

	float WaitKeyEase=0;//�C�[�Y�̃L�[�ҋ@����

	bool CursorMoveNowFlag = false;//�ړ����͉�������� false:�����ĂȂ�

	//-----------�Z���N�g�V�[��0����
	bool SSOp0_1Flag = false;//1�Ɉړ��J�n�t���O
	bool SSOp0_2Flag = false;//0����2��
	bool SSStart1_2Flag = false;//1����2��
	bool SSStart1_0Flag = false;//1����0��
	bool SStitle2_1Flag = false;//2����1��

	const float EaseValDef = 20;
	float EaseVal = EaseValDef;//�C�[�Y�l
	float DecEaseVal = 0.5;//�C�[�Y�l���炷�l
};

