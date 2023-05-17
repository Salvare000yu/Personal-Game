#include "BossAimBul.h"
#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"

#include <DirectXMath.h>

BossAimBul* BossAimBul::GetInstance()
{
	static BossAimBul instance;

	return &instance;
}

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void BossAimBul::Initialize()
{
	//定義とか仮おいておこう

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 10.0f, 10.0f, 10.0f });
	//場所

	//-------↓狙い弾↓-------//
	Nowframe = 0;//現在フレ
	GetPosOnlyFlag = true;//一度きり座標読み取りフラグ
	NowPos = {};//その時の弾位置
	boPosMoment = {};//発射時の雑魚敵位置
	MoveSp = {};//弾移動速度

	ShotTagMomOnlyFlag = true;
	ShotTagMoment = {};

	//-------↑狙い弾↑-------//
}

void BossAimBul::Update()
{
	XMFLOAT3 position = obj->GetPosition();
	position.x -= velocity.x;
	position.y -= velocity.y;
	position.z -= velocity.z;
	obj->SetPosition(position);

	//時間経過消滅
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void BossAimBul::Draw()
{
	obj->Draw();
}