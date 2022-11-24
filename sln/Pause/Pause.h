#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"
#include "BaseScene.h"

class Pause :public BaseScene, public BaseObject
{
public:
	static Pause* GetInstance();

	void Initialize()override;
	void EveryInit();
	//�|�[�Y���Ă���Ƃ�
	void PauseNow();
	//�^�C�g����
	void Update()override;
	void Draw()override;
	void DrawUI()override;
	void Finalize()override;
	void PauseGoTitle();
	void SpUpdate();
	void SpOpenPauseDraw();
	void SpFlagTrueNowDraw();
	void SpOperWindDraw();
	//���������ʂ��J���Ă���
	void OperationWind();
	//�v���C���s
	void PauseConti();
	//���������ʂ��J��
	void PauseOper();

	//-------------���������[�������[��--------------//
	//�|�[�Y���Ă邩�ǂ���
	void SetPauseFlag(bool PauseFlag) { this->PauseFlag = PauseFlag; }
	const bool& GetPauseFlag() { return PauseFlag; }
	//���������ʊJ���Ă��邩�ǂ���
	void SetOpWindOpenFlag(bool OperWindOpenFlag) { this->OperWindOpenFlag = OperWindOpenFlag; }
	const bool& GetOpWindOpenFlag() { return OperWindOpenFlag; }
	//�^�C�g���߂�t���O
	void SetSceneChangeTitleFlag(bool SceneChangeTitleFlag) { this->SceneChangeTitleFlag = SceneChangeTitleFlag; }
	const bool& GetSceneChangeTitleFlag() { return SceneChangeTitleFlag; }
	//-------------���������[�������[��--------------//

	int WaitKey0 = 0;//�O�����Ń|�[�Y�J�ł���悤��
private:
	std::unique_ptr < Sprite> sp_openpause = nullptr;
	std::unique_ptr < Sprite> sp_pause = nullptr;
	std::unique_ptr < Sprite> sp_operation = nullptr;
	std::unique_ptr < Sprite> sp_operation_wind = nullptr;
	std::unique_ptr < Sprite> sp_continuation = nullptr;
	std::unique_ptr < Sprite> sp_gotitle = nullptr;

	bool PauseFlag = false;//�|�[�Y���Ă��Ȃ�
	const float PauseSelectSizeDef = 300.0f;
	const float PauseSelectSize = 350.0f;
	float PauseNowSelect = 0;//0:������ 1:��������@2:�^�C�g��
	void  (Pause::* pFunc)();

	int WaitKeyEnter = 0;//Enter�ő���������߂�..
	bool OperWindOpenFlag = false;//�J���Ă��Ȃ�

	bool SceneChangeTitleFlag = false;//�^�C�g����
};

