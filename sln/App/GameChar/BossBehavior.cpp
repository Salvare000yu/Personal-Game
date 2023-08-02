#include "BossBehavior.h"
#include "Boss.h"
#include "GameUtility.h"

BossBehavior::BossBehavior() :
	Selector()
{
	AddChild(Node(std::bind(&BossBehavior::Appear, this)));
}

NodeResult BossBehavior::Appear()
{
	// 時間経過で終了
	if (++appearFrame > appearTotalFrame)
	{
		appearFrame = 0u; boss->SetPosition(appearEndPos);
		NodeResult::Succeeded;
	}

	// 現在の進行度
	const float rate = (float)appearFrame / (float)appearTotalFrame;

	// 目標座標に行く
	boss->SetPosition(GameUtility::UtilLerp(
		appearStartPos, appearEndPos, rate));

	return NodeResult::Running;
}
