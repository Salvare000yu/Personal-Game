#pragma once
#include "Input.h"

class ComplexInput
{
public:
	static ComplexInput* GetInstance();
	//�����ړ�
	bool RightMove();
	bool LeftMove();
	bool UpMove();
	bool DownMove();

	//�g���K�[���n
	bool tUpArrow();
	bool tDownArrow();
	bool tRightArrow();
	bool tLeftArrow();

	bool DecisionByEnter();//����
	bool PauseOpenClose();//�|�[�Y�J��
};

