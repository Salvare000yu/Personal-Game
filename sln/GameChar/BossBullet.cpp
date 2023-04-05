#include "BossBullet.h"
#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"

#include <DirectXMath.h>

BossBullet* BossBullet::GetInstance()
{
	static BossBullet instance;

	return &instance;
}

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void BossBullet::Initialize()
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

void BossBullet::Update()
{
	XMFLOAT3 position = obj->GetPosition();
	position.x -= velocity.x;
	position.y -= velocity.y;
	position.z -= velocity.z;
	obj->SetPosition(position);

	//if (TriggerR) {//リセット
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//時間経過消滅
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void BossBullet::Draw()
{
	obj->Draw();
}