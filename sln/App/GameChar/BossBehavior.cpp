#include "BossBehavior.h"
#include "Boss.h"
#include "GameUtility.h"
#include <yaml/Yaml.hpp>

using namespace DirectX;

BossBehavior::BossBehavior() :
	Sequencer()
{
	LoadYml();
	AddChild(Node(std::bind(&BossBehavior::Appear, this)));
	AddChild(Node(std::bind(&BossBehavior::Approach, this)));
	AddChild(Node(std::bind(&BossBehavior::Leave, this)));
}

void BossBehavior::LoadYml()
{
	//ymlデータ
	Yaml::Node root;
	try
	{
		Yaml::Parse(root, "Resources/charDataFile/BossBehavior.yml");
	}
	catch (...)
	{
		throw;
	}
	appearTotalFrame = root["appearTotalFrame"].As<uint32_t>();
	atkInterval = root["atkInterval"].As<uint32_t>();
	approachCountDef = root["approachCountDef"].As<uint32_t>();
	approachSpZ = root["approachSpZ"].As<float>();
	approachSpY = root["approachSpY"].As<float>();
	changeVerticalCountDef = root["changeVerticalCountDef"].As<uint16_t>();

	auto& appearStartPosNode = root["appearStartPos"];
	appearStartPos = {
		appearStartPosNode["x"].As<float>(),
		appearStartPosNode["y"].As<float>(),
		appearStartPosNode["z"].As<float>()
	};
	auto& appearEndPosNode = root["appearEndPos"];
	appearEndPos = {
		appearEndPosNode["x"].As<float>(),
		appearEndPosNode["y"].As<float>(),
		appearEndPosNode["z"].As<float>()
	};

	diffusionAtkInterval = root["diffusionAtkInterval"].As<uint32_t>();
	diffusionAtkCount = diffusionAtkInterval;
	leaveSpZ = root["leaveSpZ"].As<float>();
	leaveSpY = root["leaveSpY"].As<float>();
	leaveLim = root["leaveLim"].As<float>();
	changeVerticalNeces = root["changeVerticalNeces"].As<int16_t>();
}

NodeResult BossBehavior::Appear()
{
	// 時間経過で終了
	if (++appearFrame > appearTotalFrame)
	{
		appearFrame = 0u;
		boss->SetPosition(appearEndPos);
		return NodeResult::Succeeded;
	}

	// 現在の進行度
	const float rate = (float)appearFrame / (float)appearTotalFrame;

	// 目標座標に行く
	boss->SetPosition(GameUtility::UtilLerp(
		appearStartPos, appearEndPos, rate));

	return NodeResult::Running;
}

NodeResult BossBehavior::Approach()
{
	//---突撃---//
	//時が満ちたら
	if (++atkCount >= atkInterval) {
		//突撃時、生存時のみ発射
		if (boss->alive) { boss->Attack(); }
		//再びカウントできるように初期化
		atkCount = 0;
	}

	//敵の移動
	XMFLOAT3 position = boss->GetPosition();
	position.z -= approachSpZ;
	position.y += approachSpY;
	position.x += 7.f * std::sinf((float)approachCount / 60.f * XM_PI);
	boss->SetPosition(position);

	//ある程度近づいたら次の行動へ
	if (++approachCount >= approachCountDef) {
		approachCount = 0;
		++changeVerticalCount;//縦攻撃するためのカウント進める
		return NodeResult::Succeeded;
	}

	return NodeResult::Running;
}

NodeResult BossBehavior::Leave()
{
	//---後退---//

	//発射カウントをデクリメント
	diffusionAtkCount--;
	//時が満ちたら
	if (diffusionAtkCount == 0) {
		//生存時のみ発射
		if (boss->alive) {
			if (even_odd_NumFlag)//奇数弾
			{
				boss->DiffusionAttack();
			}
			else { boss->DiffusionAttackEavenNumber(); }
		}
		//再びカウントできるように初期化
		diffusionAtkCount = diffusionAtkInterval;
	}

	//---後退---//
	XMFLOAT3 positionBack = boss->GetPosition();
	positionBack.z += leaveSpZ;
	positionBack.y -= leaveSpY;
	boss->SetPosition(positionBack);

	//離れる制限は自機の場所に自機と離したい距離分間を開ける
	const int SpaceDistance = 400;
	leaveLim = boss->shotTag->GetPosition().z + SpaceDistance;

	//ある程度離れたら近づいてくる
	if (positionBack.z > appearEndPos.z && positionBack.y < appearEndPos.y) {
		if (even_odd_NumFlag) { even_odd_NumFlag = false; }
		else { even_odd_NumFlag = true; }

		// todo 縦攻撃
		if (changeVerticalCount == changeVerticalNeces) {//縦攻撃カウントが一定に達したら
		//	actionPattern = std::bind(&Boss::Vertical, this);
		}
		else {
			return NodeResult::Succeeded;
		}
	}

	return NodeResult::Running;
}