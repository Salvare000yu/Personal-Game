#include "Player.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

void Player::Attack()
{
	//�L�[���͎g��
	Input* input = Input::GetInstance();

	//triggerkey
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//�p�b�h�������u�Ԃ̂�
	const bool PadTriggerRB = input->TriggerButton(static_cast<int>(Button::RB));

	//�e����
	if (TriggerSPACE|| PadTriggerRB) {
		XMFLOAT3 position = obj_player->GetPosition();
		//�e����
		std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
		//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
		madeBullet->Initialize({ position });

		//�e�o�^
		bullets_.push_back(std::move(madeBullet));
	}
}

void Player::Initialize()
{
	//��`�Ƃ��������Ă�����


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

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
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
	//�p�b�h�����Ă����
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	const bool TriggerR = input->TriggerKey(DIK_R);

	time = frame / 60.f;

	//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsVanish();
		});

	//----------���ړ����� �J�����Ɠ����ɂ���
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 100;//���ɍs����͈�
	const float PlayerMinMoveLimY = 200;//��ɍs����͈�

	const float PlayerMaxMoveLimZ = 290;//���
	const float PlayerMinMoveLimZ = 200;

	XMFLOAT3 position = obj_player->GetPosition();
	position.x = max(position.x, -PlayerMoveLimX);
	position.x = min(position.x, +PlayerMoveLimX);
	position.y = max(position.y, -PlayerMaxMoveLimY);//���ɍs����͈�
	position.y = min(position.y, +PlayerMinMoveLimY);//��ɍs����͈�
	position.z = max(position.z, -PlayerMaxMoveLimZ);
	position.z = min(position.z, +PlayerMinMoveLimZ);
	obj_player->SetPosition(position);
	//----------���ړ�����

	//------------------���v���C���[�ړ����p��
	if (inputW || inputS || inputA || inputD || inputQ || inputZ|| PadInputUP|| PadInputDOWN|| PadInputLEFT|| PadInputRIGHT)
	{

		//------�v���C���[�������ړ�------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 1;

		if ((inputS)|| PadInputDOWN) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x <= 10) {
				rotation.x += 1.f;
			}
			obj_player->SetRotation(rotation);

		}

		if ((inputW) || PadInputUP) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x >= -10) {
				rotation.x -= 1.f;
			}
			obj_player->SetRotation(rotation);
		}

		if ((inputA) || PadInputLEFT) {

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

		if ((inputD) || PadInputRIGHT) {

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
	if(TriggerR){//���Z�b�g
		obj_player->SetPosition({ 0,40,-170 });
		obj_player->SetRotation({ 0,0,0 });
	}

	//------------------���v���C���[�ړ����p��

	//������]
	if (inputE)
	{
		XMFLOAT3 rotation = obj_player->GetRotation();
		rotation.y++;
		obj_player->SetRotation(rotation);
	}

	//���ˏ���
	Attack();
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Update();
	}

	obj_player->Update();

}

void Player::Draw()
{
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Draw();
	}

	obj_player->Draw();
}
