#pragma once
#include "Input.h"

class ComplexInput
{
public:
	static ComplexInput* GetInstance();
	bool RightMove();
	bool UpMove();
	bool DownMove();
	bool DecisionByEnter();
	bool LeftMove();
};

