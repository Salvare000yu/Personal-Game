#include "Enemy.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

Enemy* Enemy::GetInstance()
{
	static Enemy instance;

	return &instance;
}

void Enemy::Initialize()
{
	//使う定義とか　仮おいとくね
	time = frame / 60.f;	// 60fps想定

	//もでる読み込み
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
	//作る
	obj_enemy.reset(Object3d::Create());
	//セット
	obj_enemy->SetModel(mod_enemy.get());
	//-----↓任意↓-----//
	//大きさ
	obj_enemy->SetScale({ 20.0f, 20.0f, 20.0f });
	//場所
	obj_enemy->SetPosition({ 0,100,-0 });
}

void Enemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	if (input3) {
		XMFLOAT3 position = obj_enemy->GetPosition();
		position.z += 5;
		obj_enemy->SetPosition(position);
	}

	//黄金の回転
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 rotation = obj_enemy->GetRotation();
		rotation.y += 0.7f;
		rotation.x += 0.4f;
		obj_enemy->SetRotation({ rotation });
	}

	obj_enemy->Update();
}

void Enemy::Draw()
{
	obj_enemy->Draw();
}
