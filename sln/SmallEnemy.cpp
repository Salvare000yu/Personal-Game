#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>
#include <random>std::srand(std::time(nullptr));

void SmallEnemy::Initialize()
{

	//もでる読み込み
	mod_smallenemy.reset(Model::LoadFromOBJ("SmallEnemy"));
	//作る
	obj.reset(Object3d::Create());
	//セット
	obj->SetModel(mod_smallenemy.get());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	//場所

	SEneRandX = rand() % 100 - 50;
	obj->SetPosition({ SEneRandX,40,400 });
}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	//srand((unsigned int)timeGetTime());
	//SEneRandX = SEneMinX + (int)(rand() * (SEneMaxX - SEneMinX + 1) / (1 + RAND_MAX));
	//XMFLOAT3 position = obj->GetPosition();
	//position.x = SEneRandX;
	//obj->SetPosition(position);

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 smEnemPos = obj->GetPosition();
		smEnemPos.z -= 4;
		//---静的メンバ変数初期化　弾の座標を当たり判定で使う
		XMFLOAT3 SmallEnemyPosMemory = {};
		SmallEnemyPosMemory = obj->GetPosition();//判定のためポジション入れる
		obj->SetPosition(smEnemPos);

	}

	//時間経過消滅
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void SmallEnemy::Draw()
{

	obj->Draw();
}
