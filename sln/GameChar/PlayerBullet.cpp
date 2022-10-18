#include "PlayerBullet.h"
#include "Input.h"

#include <DirectXMath.h>

using namespace DirectX;

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void PlayerBullet::Initialize()
{
	//��`�Ƃ��������Ă�����
	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 3.f, 3.f, 3.f });
	//�ꏊ

	//obj->SetPosition({ BulletPos });

}

void PlayerBullet::Update()
{
	XMFLOAT3 BulletPos = obj->GetPosition();
	//�e���ˑ��x
	BulletPos.z += velocity.z;
	BulletPos.x += velocity.x;
	BulletPos.y += velocity.y;

	obj->SetPosition(BulletPos);

	//if (TriggerR) {//���Z�b�g
	//	obj->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void PlayerBullet::Draw()
{
	obj->Draw();
}
