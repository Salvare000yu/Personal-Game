#include "SmallEnemyBullet.h"
#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"
#include "Player.h"

#include <DirectXMath.h>

SmallEnemyBullet* SmallEnemyBullet::GetInstance()
{
	static SmallEnemyBullet instance;

	return &instance;
}

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void SmallEnemyBullet::Initialize()
{
	//定義とか仮おいておこう

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 8.0f, 8.0f, 8.0f });
	//場所
	//obj->SetPosition({ position });

	//-------↓狙い弾↓-------//
	Nowframe = 0;//現在フレ
	GetPosOnlyFlag = true;//一度きり座標読み取りフラグ
	NowPos = {};//その時の弾位置
	sePosMoment = {};//発射時の雑魚敵位置
	MoveSp = {};//弾移動速度

	ShotTagMomOnlyFlag = true;
	ShotTagMoment = {};

	//-------↑狙い弾↑-------//
}

void SmallEnemyBullet::Update()
{
	XMFLOAT3 sePos = obj->GetPosition();
	sePos.x -= velocity.x;
	sePos.y -= velocity.y;
	sePos.z -= velocity.z;
	obj->SetPosition(sePos);

	//if (TriggerR) {//リセット
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//時間経過消滅
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void SmallEnemyBullet::Draw()
{
	obj->Draw();
}