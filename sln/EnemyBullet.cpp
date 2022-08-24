#include "EnemyBullet.h"
#include "Enemy.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

EnemyBullet* EnemyBullet::GetInstance()
{
	static EnemyBullet instance;

	return &instance;
}

void EnemyBullet::OnCollision()
{
	isVanish_ = TRUE;
}

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void EnemyBullet::Initialize(DirectX::XMFLOAT3 position)
{
	//定義とか仮おいておこう

	//もでる読み込み
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	//作る
	obj_enemybullet.reset(Object3d::Create());
	//セット
	obj_enemybullet->SetModel(mod_enemybullet.get());
	//-----↓任意↓-----//
	//大きさ
	obj_enemybullet->SetScale({ 2.0f, 2.0f, 2.0f });
	//場所
	obj_enemybullet->SetPosition({ position });

}

void EnemyBullet::Update()
{

	Input* input = Input::GetInstance();

	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);

	const bool TriggerR = input->TriggerKey(DIK_R);

	XMFLOAT3 position = obj_enemybullet->GetPosition();
	position.z = position.z - 2;
	obj_enemybullet->SetPosition(position);

	//if (TriggerR) {//リセット
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//時間経過消滅
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj_enemybullet->Update();
}

void EnemyBullet::Draw()
{
	obj_enemybullet->Draw();
}
