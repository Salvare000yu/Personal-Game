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

	uint32_t diffusionAtkInterval;
	uint32_t diffusionAtkCount = 0;

	bool even_odd_NumFlag = true;//奇数弾

	//------縦に揺れる攻撃F
	uint16_t changeVerticalCountDef;//デフォ0　終わったら消してん
	uint16_t changeVerticalCount{};

	DirectX::XMFLOAT3 appearStartPos{};
	DirectX::XMFLOAT3 appearEndPos{};

	//離れる速さ
	float leaveSpZ;
	float leaveSpY;
	//離れられる距離
	float leaveLim;
	//離れる制限は自機の場所に自機と離したい距離分間を開ける
	uint16_t SpaceDistance;

	int16_t changeVerticalNeces;//縦攻撃に移る為に必要カウント

	//最初の上昇値
	int16_t startVerticalValDef;
	int16_t startVerticalVal;

	float upStartPosY;//上昇開始Y座標

	//当たり判定取るか true:取る
	bool doCollision = true;

	//待ち時間
	int16_t verticalWaitCountDef;
	int16_t verticalWaitCount;

	//UpDown何回ループさせるか
	int16_t verticalLoopCountDef;
	int16_t verticalLoopCount;
	//次の動き false:これじゃない
	bool nextUp = true;//最初
	bool nextDown = false;

public:
	BossBehavior();

	inline void SetBoss(Boss* boss) { this->boss = boss; }

private:

	void LoadYml();

	NodeResult Appear();
	NodeResult Approach();
	NodeResult Leave();
	NodeResult StartVertical();
	NodeResult VerticalWait();
};

