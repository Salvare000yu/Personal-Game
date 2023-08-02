#pragma once
#include "BaseComposite.h"

// ビヘイビアツリーのシーケンサー
class Sequencer :
	public BaseComposite
{
	NodeResult MainFunc() override;
public:
	using BaseComposite::BaseComposite;
};

