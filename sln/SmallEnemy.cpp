#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>
#include <random>std::srand(std::time(nullptr));

SmallEnemy* SmallEnemy::GetInstance()
{
	static SmallEnemy instance;

	return &instance;
}

DirectX::XMFLOAT3 SmallEnemy::GetSmallEnemyPosMemory()
{
	XMFLOAT3 SmallEnemyPosMemory = {};
	return SmallEnemyPosMemory;
}

void SmallEnemy::Initialize()
{

	//もでる読み込み
	mod_smallenemy.reset(Model::LoadFromOBJ("SmallEnemy"));
	//作る
	obj_smallenemy.reset(Object3d::Create());
	//セット
	obj_smallenemy->SetModel(mod_smallenemy.get());
	//-----↓任意↓-----//
	//大きさ
	obj_smallenemy->SetScale({ 5.0f, 5.0f, 5.0f });
	//場所

	SEneRandX = rand() % 100 - 50;
	obj_smallenemy->SetPosition({ SEneRandX,40,400 });
}

void SmallEnemy::OnCollision()
{
	//isVanish_ = TRUE;
}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	//srand((unsigned int)timeGetTime());
	//SEneRandX = SEneMinX + (int)(rand() * (SEneMaxX - SEneMinX + 1) / (1 + RAND_MAX));
	//XMFLOAT3 position = obj_smallenemy->GetPosition();
	//position.x = SEneRandX;
	//obj_smallenemy->SetPosition(position);

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 smEnemPos = obj_smallenemy->GetPosition();
		smEnemPos.z -= 4;
		//---静的メンバ変数初期化　弾の座標を当たり判定で使う
		XMFLOAT3 SmallEnemyPosMemory = {};
		SmallEnemyPosMemory = obj_smallenemy->GetPosition();//判定のためポジション入れる
		obj_smallenemy->SetPosition(smEnemPos);

	}

	//時間経過消滅
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj_smallenemy->Update();
}

void SmallEnemy::Draw()
{

	obj_smallenemy->Draw();
}
