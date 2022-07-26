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

	//���ł�ǂݍ���
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
	//���
	obj_enemy.reset(Object3d::Create());
	//�Z�b�g
	obj_enemy->SetModel(mod_enemy.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_enemy->SetScale({ 20.0f, 20.0f, 20.0f });
	//�ꏊ
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

	//�����̉�]
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

	obj_enemy->Update();
}

void Enemy::Draw()
{
	obj_enemy->Draw();
}
