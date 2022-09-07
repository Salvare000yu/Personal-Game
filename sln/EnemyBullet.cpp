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

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void EnemyBullet::Initialize()
{
	//��`�Ƃ��������Ă�����
	
	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 2.0f, 2.0f, 2.0f });
	//�ꏊ
	//obj->SetPosition({ position });

}

void EnemyBullet::Update()
{

	XMFLOAT3 position = obj->GetPosition();
	position.z = position.z - 2;
	obj->SetPosition(position);

	//if (TriggerR) {//���Z�b�g
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void EnemyBullet::Draw()
{
	obj->Draw();
}
