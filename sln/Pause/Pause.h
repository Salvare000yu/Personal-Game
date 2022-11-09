#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"

class Pause :public BaseScene, public BaseObject
{
public:
	static Pause* GetInstance();

	void Initialize()override;
	//�|�[�Y���Ă���Ƃ�
	void PauseNow();
	//�^�C�g����
	void PauseGoTitle();
	//���������ʂ��J���Ă���
	void OperationWind();
	//�v���C���s
	void PauseConti();
	//���������ʂ��J��
	void PauseOper();

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

	int WaitKey0 = 0;//�O�����Ń|�[�Y�J�ł���悤��
	int WaitKeyEnter = 0;//Enter�ő���������߂�..
	bool OperWindOpenFlag = false;//�J���Ă��Ȃ�
};

