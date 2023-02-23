#include "BossStraightBul.h"
#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"

#include <DirectXMath.h>

BossStraightBul* BossStraightBul::GetInstance()
{
	static BossStraightBul instance;

	return &instance;
}

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void BossStraightBul::Initialize()
{
	//定義とか仮おいておこう

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 10.0f, 10.0f, 10.0f });
	//場所
	//obj->SetPosition({ position });


}

void BossStraightBul::Update()
{

	XMFLOAT3 position = obj->GetPosition();
	position.x -= velocity.x;
	position.y -= velocity.y;
	position.z -= velocity.z;
	obj->SetPosition(position);

	if (StraightBulSp < StraightBulSpLim) {//速度制限内なら加速
		StraightBulSp += StraightBulSp * StraightBulAccel;
	}
	velocity.z = StraightBulSp;

	//時間経過消滅
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void BossStraightBul::Draw()
{
	obj->Draw();
}
