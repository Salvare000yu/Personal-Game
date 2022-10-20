#include "PlayerFireLine.h"

using namespace DirectX;

void PlayerFireLine::Initialize()
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

void PlayerFireLine::Update()
{
	XMFLOAT3 FireLinePos = obj->GetPosition();

	obj->SetPosition(FireLinePos);

	obj->Update();
}

void PlayerFireLine::Draw()
{
	obj->Draw();
}
