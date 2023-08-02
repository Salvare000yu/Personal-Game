#pragma once
#include "BaseComposite.h"

// ビヘイビアツリーのセレクター
class Selector :
	public BaseComposite
{
	NodeResult MainFunc() override;
public:
	using BaseComposite::BaseComposite;
};
