#include "BossBullet.h"
#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"

#include <DirectXMath.h>

BossBullet* BossBullet::GetInstance()
{
	static BossBullet instance;

	return &instance;
}

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void BossBullet::Initialize()
{
	//��`�Ƃ��������Ă�����
	
	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 10.0f, 10.0f, 10.0f });
	//�ꏊ
	//obj->SetPosition({ position });

	//-------���_���e��-------//
	Nowframe = 0;//���݃t��
	GetPosFlag = true;//��x������W�ǂݎ��t���O
	NowPos = {};//���̎��̒e�ʒu
	boPosMoment = {};//���ˎ��̎G���G�ʒu
	MoveSp = {};//�e�ړ����x

	ShotTagMomFlag = true;
	ShotTagMoment = {};

	//-------���_���e��-------//

}

void BossBullet::Update()
{

	XMFLOAT3 position = obj->GetPosition();
	position.x -= velocity.x;
	position.y -= velocity.y;
	position.z -= velocity.z;
	obj->SetPosition(position);

	//if (TriggerR) {//���Z�b�g
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void BossBullet::Draw()
{
	obj->Draw();
}
