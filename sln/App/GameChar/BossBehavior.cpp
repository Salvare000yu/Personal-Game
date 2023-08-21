﻿#include "BossBehavior.h"
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
	AddChild(Node(std::bind(&BossBehavior::StartVertical, this)));
	AddChild(Node(std::bind(&BossBehavior::VerticalWait, this)));
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
	SpaceDistance = root["SpaceDistance"].As<uint16_t>();
	startVerticalValDef = root["startVerticalValDef"].As<int16_t>();
	startVerticalVal = startVerticalValDef;
	upStartPosY = root["upStartPosY"].As<float>();
	verticalWaitCountDef = root["verticalWaitCountDef"].As<int16_t>();
	verticalWaitCount = verticalWaitCountDef;
	verticalLoopCountDef = root["verticalLoopCountDef"].As<int16_t>();
	verticalLoopCount = verticalLoopCountDef;
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

NodeResult BossBehavior::StartVertical()
{

	XMFLOAT3 position = boss->GetPosition();
	//まずは上昇
	position.y += startVerticalVal;
	startVerticalVal--;

	if (position.y < upStartPosY) {//一定超えたら判定切ってから待ち
		startVerticalVal = startVerticalValDef;//最初の上昇値戻す
		return NodeResult::Succeeded;
	}
	boss->SetPosition(position);

	return NodeResult::Running;
}

NodeResult BossBehavior::VerticalWait()
{
	doCollision = false;//Wait時は画面外なので当たり判定取らない
	verticalWaitCount--;//待ちカウント減らす

	if (verticalWaitCount == 0) {
		verticalWaitCount = verticalWaitCountDef / 2;//初回以降はデフォ/2にカウント戻す
		doCollision = true;//当たり判定オンにしてから次行動

		if (verticalLoopCount != 0) {//UpDownループ回数が0でなければ、継続
			if (nextDown) {//次下降なら
				nextDown = false;//戻してから
				// todo 下移動
				//verticalPattern = std::bind(&Boss::VerticalDown, this);//下移動
			}
			if (nextUp) {//次上昇なら
				nextUp = false;//戻してから
				// todo 上移動
				return NodeResult::Succeeded;
				//verticalPattern = std::bind(&Boss::VerticalUp, this);//上移動
			}
		}
		else {//るーぷが指定数おわったら（カウント0で）
			//次の動き戻す
			nextUp = false;
			nextDown = true;//最初は下降
			verticalLoopCount = verticalLoopCountDef;
			// todo Reverseへ
			//verticalPattern = std::bind(&Boss::VerticalReverse, this);//戻るパターンへ
		}
	}

	return NodeResult::Running;
}
