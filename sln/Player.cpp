#include "Player.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

void Player::Initialize()
{

	//���ł�ǂݍ���
	mod_player.reset(Model::LoadFromOBJ("hiyoko"));
	//���
	obj_player.reset(Object3d::Create());
	//�Z�b�g
	obj_player->SetModel(mod_player.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_player->SetScale({ 3.0f, 3.0f, 3.0f });
	//�ꏊ
	obj_player->SetPosition({ 0,40,-170 });
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);

	time = frame / 60.f;

	//------------------���v���C���[�ړ����p��
	if (inputW || inputS || inputA || inputD || inputQ || inputZ)
	{

		//------�v���C���[�������ړ�------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 1;

		if (inputS) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x <= 10) {
				rotation.x += 1.f;
			}
			obj_player->SetRotation(rotation);

		}

		if (inputW) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x >= -10) {
				rotation.x -= 1.f;
			}
			obj_player->SetRotation(rotation);
		}

		if (inputA) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.x = position.x - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if (inputD) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.x = position.x + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if (inputQ) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.y = position.y + moveSpeed;
			obj_player->SetPosition(position);
		}

		if (inputZ) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.y = position.y - moveSpeed;
			obj_player->SetPosition(position);
		}

	}
	//------------------���v���C���[�ړ����p��

	//������]
	if (inputE)
	{
		XMFLOAT3 rotation = obj_player->GetRotation();
		rotation.y++;
		obj_player->SetRotation(rotation);
	}

	obj_player->Update();

}

void Player::Draw()
{
	obj_player->Draw();
}
