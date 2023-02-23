#include "BossStraightBul.h"
#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "DebugText.h"

#include <DirectXMath.h>

BossStraightBul* BossStraightBul::GetInstance()
{
	static BossStraightBul instance;

	return &instance;
}

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void BossStraightBul::Initialize()
{
	//��`�Ƃ��������Ă�����

	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 10.0f, 10.0f, 10.0f });
	//�ꏊ
	//obj->SetPosition({ position });


}

void BossStraightBul::Update()
{

	XMFLOAT3 position = obj->GetPosition();
	position.x -= velocity.x;
	position.y -= velocity.y;
	position.z -= velocity.z;
	obj->SetPosition(position);

	if (StraightBulSp < StraightBulSpLim) {//���x�������Ȃ����
		StraightBulSp += StraightBulSp * StraightBulAccel;
	}
	velocity.z = StraightBulSp;

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void BossStraightBul::Draw()
{
	obj->Draw();
}
