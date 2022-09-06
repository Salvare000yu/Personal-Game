#include "EnemyBullet.h"
#include "Enemy.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"

#include <DirectXMath.h>

EnemyBullet* EnemyBullet::GetInstance()
{
	static EnemyBullet instance;

	return &instance;
}

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void EnemyBullet::Initialize()
{
	//定義とか仮おいておこう

	//もでる読み込み
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	//作る
	obj.reset(Object3d::Create());
	//セット
	obj->SetModel(mod_enemybullet.get());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 2.0f, 2.0f, 2.0f });
	//場所
	//obj->SetPosition({ position });

}

void EnemyBullet::Update()
{

	XMFLOAT3 position = obj->GetPosition();
	position.z = position.z - 2;
	obj->SetPosition(position);

	//if (TriggerR) {//リセット
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//時間経過消滅
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj->Update();
}

void EnemyBullet::Draw()
{
	obj->Draw();
}
