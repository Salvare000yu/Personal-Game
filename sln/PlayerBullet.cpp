#include "PlayerBullet.h"
#include "Player.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

PlayerBullet* PlayerBullet::GetInstance()
{
	static PlayerBullet instance;

	return &instance;
}


void PlayerBullet::Initialize()
{
	//��`�Ƃ��������Ă�����

	//���ł�ǂݍ���
	mod_playerbullet.reset(Model::LoadFromOBJ("bullet"));
	//���
	obj_playerbullet.reset(Object3d::Create());
	//�Z�b�g
	obj_playerbullet->SetModel(mod_playerbullet.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_playerbullet->SetScale({ 2.0f, 2.0f, 2.0f });
	//�ꏊ
	//XMFLOAT3 position = obj_playerbullet->GetPosition();
	�v���C���[���ŕϐ�����Ă����ɍ��W����A������g���Ēe���W
	bulletposx = playerposx;
	bulletposy = playerposy;
	bulletposz = playerposz

	obj_playerbullet->SetPosition({bulletposition });

}

void PlayerBullet::Update()
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

	if (TriggerR) {//���Z�b�g
		obj_playerbullet->SetPosition({ 0,40,-170 });
	}

	obj_playerbullet->Update();
}

void PlayerBullet::Draw()
{
	obj_playerbullet->Draw();
}
