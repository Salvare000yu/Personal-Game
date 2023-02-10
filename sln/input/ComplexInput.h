#pragma once
#include "Input.h"

class ComplexInput
{
public:
	static ComplexInput* GetInstance();
	//方向移動
	bool RightMove();
	bool LeftMove();
	bool UpMove();
	bool DownMove();

	bool tLeftMove();

	bool tRightMove();

	bool tUpMove();

	bool tDownMove();

	//トリガー矢印系
	bool tUpArrow();
	bool tDownArrow();
	bool tRightArrow();
	bool tLeftArrow();

	bool PlayerDash();//自機ダッシュ

	bool DecisionByEnter();//決定
	bool PauseOpenClose();//ポーズ開閉
};

