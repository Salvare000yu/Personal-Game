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
	obj_enemy->SetPosition({ -100,100,-50 });
}

void Enemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}

	//黄金の回転
	for (int i = 0; i < 1; i++)
	{
		frame += 1.f;

		XMFLOAT3 rotation = obj_enemy->GetRotation();
		rotation.y += 0.7f;
		rotation.x += 0.4f;
		obj_enemy->SetRotation({ rotation });

		XMFLOAT3 position = obj_enemy->GetPosition();
		position.x += 5.f*sin(time * 3.14159265358f);
		obj_enemy->SetPosition(position);
	}
	//----------------------------------------------↓関数化しろボケ
	switch (actionPattern_) {
	case ActionPattern::Approach://近づくパターン
	default:
		//突撃
		XMFLOAT3 position = obj_enemy->GetPosition();
		position.z -= ApproachSp;
		obj_enemy->SetPosition(position);

		//ある程度近づいたらキモ過ぎて離れる
		if (position.z == ApproachLim) {
			actionPattern_ = ActionPattern::Leave;
		}
		break;
	case ActionPattern::Leave://後退パターン
		//後退
		XMFLOAT3 positionBack = obj_enemy->GetPosition();
		positionBack.z += ApproachSp;
		obj_enemy->SetPosition(positionBack);

		//ある程度離れたら近づいてくる
		if (positionBack.z == LeaveLim) {
			actionPattern_ = ActionPattern::Approach;
		}
		break;
	}
	//----------------------------------------------↑関数化しろボケ

	obj_enemy->Update();
}

void Enemy::Draw()
{
	obj_enemy->Draw();
}
