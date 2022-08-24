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

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void EnemyBullet::Initialize(DirectX::XMFLOAT3 position)
{
	//��`�Ƃ��������Ă�����

	//���ł�ǂݍ���
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	//���
	obj_enemybullet.reset(Object3d::Create());
	//�Z�b�g
	obj_enemybullet->SetModel(mod_enemybullet.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_enemybullet->SetScale({ 2.0f, 2.0f, 2.0f });
	//�ꏊ
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

	//if (TriggerR) {//���Z�b�g
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj_enemybullet->Update();
}

void EnemyBullet::Draw()
{
	obj_enemybullet->Draw();
}
