#pragma once
#include "Engine/BehaviorTree/Selector.h"

#include <DirectXMath.h>
class Boss;

class BossBehavior :
	public Selector
{
	Boss* boss = nullptr;

	uint32_t appearTotalFrame;
	uint32_t appearFrame = 0;

	DirectX::XMFLOAT3 appearStartPos{};
	DirectX::XMFLOAT3 appearEndPos{};

public:
	BossBehavior();

	inline void SetBoss(Boss* boss) { this->boss = boss; }
	inline void SetAppearTotalFrame(uint32_t appearTotalFrame) { this->appearTotalFrame = appearTotalFrame; }

	inline void SetAppearStartPos(const DirectX::XMFLOAT3& appearStartPos) { this->appearStartPos = appearStartPos; }
	inline void SetAppearEndPos(const DirectX::XMFLOAT3& appearEndPos) { this->appearEndPos = appearEndPos; }

private:
	NodeResult Appear();
};

