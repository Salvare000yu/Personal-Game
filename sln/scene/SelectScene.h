#pragma once
/// <summary>
/// �Z���N�g���
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include <memory>
#include "../DxBase.h"

class SelectScene :public BaseScene
{
public:

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
};

