#pragma once
#include "Input.h"
/// <summary>
/// �L�[�����ꂽ���������������Ă��킩��₷������
/// </summary>

class ComplexInput
{
public:
	static ComplexInput* GetInstance();
	//�����ړ�
	bool RightMove();
	bool LeftMove();
	bool UpMove();
	bool DownMove();

	bool tLeftMove();

	bool tRightMove();

	bool tUpMove();

	bool tDownMove();

	//�g���K�[���n
	bool tUpArrow();
	bool tDownArrow();
	bool tRightArrow();
	bool tLeftArrow();

	bool PlayerDash();//���@�_�b�V��

	bool Decision();//����
	bool PauseOpenClose();//�|�[�Y�J��
};
