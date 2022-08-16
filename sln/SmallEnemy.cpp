#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

SmallEnemy* SmallEnemy::GetInstance()
{
	static SmallEnemy instance;

	return &instance;
}

void SmallEnemy::Initialize()
{

	//���ł�ǂݍ���
	mod_smallenemy.reset(Model::LoadFromOBJ("SmallEnemy"));
	//���
	obj_smallenemy.reset(Object3d::Create());
	//�Z�b�g
	obj_smallenemy->SetModel(mod_smallenemy.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_smallenemy->SetScale({ 5.0f, 5.0f, 5.0f });
	//�ꏊ
	obj_smallenemy->SetPosition({ 0,150,50 });
}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 position = obj_smallenemy->GetPosition();
		position.z -= 1;
		obj_smallenemy->SetPosition(position);

	}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj_smallenemy->Update();
}

void SmallEnemy::Draw()
{

	obj_smallenemy->Draw();
}
