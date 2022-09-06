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

void EnemyBullet::OnCollision()
{
	//DebugText::GetInstance()->Print("EnemyBulletCollision", 180, 180, 5);
	//isVanish_ = TRUE;
}

DirectX::XMFLOAT3 EnemyBullet::GetEnemyBulPosMemory()
{
	XMFLOAT3 EnemyBulPosMemory = {};
	return EnemyBulPosMemory;
}

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void EnemyBullet::Initialize()
{
	//��`�Ƃ��������Ă�����

	//���ł�ǂݍ���
	mod_enemybullet.reset(Model::LoadFromOBJ("EnemBul"));
	//���
	obj.reset(Object3d::Create());
	//�Z�b�g
	obj->SetModel(mod_enemybullet.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 2.0f, 2.0f, 2.0f });
	//�ꏊ
	//obj->SetPosition({ position });

}

void EnemyBullet::Update()
{

	XMFLOAT3 position = obj->GetPosition();
	//---�ÓI�����o�ϐ��������@�G�e�̍��W�𓖂��蔻��Ŏg��
	XMFLOAT3 EnemyBulPosMemory = {};
	EnemyBulPosMemory = obj->GetPosition();//����̂��߃|�W�V���������
	position.z = position.z - 2;
	obj->SetPosition(position);

	//if (TriggerR) {//���Z�b�g
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj->Update();
}

void EnemyBullet::Draw()
{
	obj->Draw();
}
