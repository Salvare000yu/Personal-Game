#include "SmallEnemyBullet.h"
#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"
#include "Player.h"

#include <DirectXMath.h>

SmallEnemyBullet* SmallEnemyBullet::GetInstance()
{
	static SmallEnemyBullet instance;

	return &instance;
}

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void SmallEnemyBullet::Initialize()
{
	//��`�Ƃ��������Ă�����

	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 8.0f, 8.0f, 8.0f });
	//�ꏊ
	//obj->SetPosition({ position });

	//-------���_���e��-------//
	Nowframe = 0;//���݃t��
	GetPosOnlyFlag = true;//��x������W�ǂݎ��t���O
	NowPos = {};//���̎��̒e�ʒu
	sePosMoment = {};//���ˎ��̎G���G�ʒu
	MoveSp = {};//�e�ړ����x

	ShotTagMomOnlyFlag = true;
	ShotTagMoment = {};

	//-------���_���e��-------//
}

void SmallEnemyBullet::Update()
{
	XMFLOAT3 sePos = obj->GetPosition();
	sePos.x -= velocity.x;
	sePos.y -= velocity.y;
	sePos.z -= velocity.z;
	obj->SetPosition(sePos);

	//if (TriggerR) {//���Z�b�g
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void SmallEnemyBullet::Draw()
{
	obj->Draw();
}