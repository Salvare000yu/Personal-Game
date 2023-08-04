#pragma once
#include "Engine/BehaviorTree/Sequencer.h"

#include <DirectXMath.h>

class Boss;

class BossBehavior :
	public Sequencer
{
	Boss* boss = nullptr;

	uint32_t appearTotalFrame;
	uint32_t appearFrame = 0;

	uint32_t atkCount = 0;
	//フレームごとに発射
	uint32_t atkInterval;

	//この時間だけこの行動をする
	uint16_t approachCountDef;
	uint16_t approachCount{};

	//近づく速さ
	float approachSpZ;
	float approachSpY;

	//------縦に揺れる攻撃F
	uint16_t changeVerticalCountDef;//デフォ0　終わったら消してん
	uint16_t changeVerticalCount{};

	DirectX::XMFLOAT3 appearStartPos{};
	DirectX::XMFLOAT3 appearEndPos{};

public:
	BossBehavior();

	inline void SetBoss(Boss* boss) { this->boss = boss; }

private:

	void LoadYml();

	NodeResult Appear();
	NodeResult Approach();
};

