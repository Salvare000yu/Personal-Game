#include "Player.h"
//#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"

#include <DirectXMath.h>

using namespace DirectX;

void Player::Attack()
{
	//�L�[���͎g��
	Input* input = Input::GetInstance();

	//triggerkey
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//�p�b�h�������u�Ԃ̂�
	const bool PadTriggerRB = input->TriggerButton(static_cast<int>(Button::RB));

	const bool TriggerMouseLEFT = input->TriggerMouse(0);

	//�e����
	if ((TriggerSPACE || PadTriggerRB || TriggerMouseLEFT)&&AttackIntervalFlag==false) {
		XMFLOAT3 PlayerPos = obj->GetPosition();
		//�e����
		std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
		//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
		madeBullet->Initialize();
		madeBullet->SetModel(pBulModel);
		madeBullet->SetPosition(PlayerPos);

		// velocity���Z�o
		XMVECTOR vecvelocity = XMVectorSet(0, 0, 3, 0);
		XMFLOAT3 xmfloat3velocity;
		XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

		madeBullet->SetVelocity(xmfloat3velocity);

		// �����Đ� �炵�����Ƃ�
		GameSound::GetInstance()->PlayWave("shot.wav", 0.3f);

		//�e�o�^
		bullets_.push_back(std::move(madeBullet));

		AttackIntervalFlag = true;

	}
	if (AttackIntervalFlag == true)
	{
		if (--AtkInterval_ >= 0) {//�N�[���^�C�� 0�܂Ō��炷	

			//XMFLOAT3 PlayerPos = obj->GetPosition();
			////�e����
			//std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
			////bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
			//madeBullet->Initialize();
			//madeBullet->SetModel(pBulModel);
			//madeBullet->SetPosition(PlayerPos);

			//// velocity���Z�o
			//XMVECTOR vecvelocity = XMVectorSet(0, 0, 3, 0);
			//XMFLOAT3 xmfloat3velocity;
			//XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

			//madeBullet->SetVelocity(xmfloat3velocity);

			//// �����Đ� �炵�����Ƃ�
			//GameSound::GetInstance()->PlayWave("shot.wav", 0.3f);

			////�e�o�^
			//bullets_.push_back(std::move(madeBullet));

			if (AtkInterval_ <= 0) {
				AttackIntervalFlag = false;
			}//0�Ȃ����炭�炢��ԉ���
		}else { AtkInterval_ = AtkInterval; }
	}

}

void Player::Initialize()
{
	//��`�Ƃ��������Ă�����


	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 3.0f, 3.0f, 3.0f });
	//�ꏊ
	obj->SetPosition({ 0,40,-170 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("shot.wav");
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	//const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	//const bool inputC = input->PushKey(DIK_C);
	const bool inputI = input->PushKey(DIK_I);
	const bool inputJ = input->PushKey(DIK_J);
	const bool inputK = input->PushKey(DIK_K);
	const bool inputL = input->PushKey(DIK_L);
	const bool inputUp = input->PushKey(DIK_UP);
	const bool inputDown = input->PushKey(DIK_DOWN);
	const bool inputRight = input->PushKey(DIK_RIGHT);
	const bool inputLeft = input->PushKey(DIK_LEFT);
	//�p�b�h�����Ă����
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	const bool TriggerR = input->TriggerKey(DIK_R);

	//time = frame / 60.f;

	//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
		});

	//----------���ړ����� �J�����Ɠ����ɂ���
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 100;//���ɍs����͈�
	const float PlayerMinMoveLimY = 200;//��ɍs����͈�

	const float PlayerMaxMoveLimZ = 290;//���
	const float PlayerMinMoveLimZ = 200;

	XMFLOAT3 PlayerPos = obj->GetPosition();
	PlayerPos.x = max(PlayerPos.x, -PlayerMoveLimX);
	PlayerPos.x = min(PlayerPos.x, +PlayerMoveLimX);
	PlayerPos.y = max(PlayerPos.y, -PlayerMaxMoveLimY);//���ɍs����͈�
	PlayerPos.y = min(PlayerPos.y, +PlayerMinMoveLimY);//��ɍs����͈�
	PlayerPos.z = max(PlayerPos.z, -PlayerMaxMoveLimZ);
	PlayerPos.z = min(PlayerPos.z, +PlayerMinMoveLimZ);
	obj->SetPosition(PlayerPos);
	//----------���ړ�����

	//------------------���v���C���[�ړ����p��
	if (inputW || inputS || inputA || inputD || inputQ || inputZ || PadInputUP || PadInputDOWN || PadInputLEFT || PadInputRIGHT)
	{

		//------�v���C���[�������ړ�------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 2.f;

		if ((inputS) || PadInputDOWN) {

			XMFLOAT3 PlayerPos = obj->GetPosition();
			PlayerPos.z = PlayerPos.z - moveSpeed;
			obj->SetPosition(PlayerPos);

			//XMFLOAT3 rotation = obj->GetRotation();
			//if (rotation.x <= 10) {
			//	rotation.x += 1.f;
			//}
			//obj->SetRotation(rotation);

		}

		if ((inputW) || PadInputUP) {

			XMFLOAT3 PlayerPos = obj->GetPosition();
			PlayerPos.z = PlayerPos.z + moveSpeed;
			obj->SetPosition(PlayerPos);

			//XMFLOAT3 rotation = obj->GetRotation();
			//if (rotation.x >= -10) {
			//	rotation.x -= 1.f;
			//}
			//obj->SetRotation(rotation);
		}

		if ((inputA) || PadInputLEFT) {

			XMFLOAT3 PlayerPos = obj->GetPosition();
			PlayerPos.x = PlayerPos.x - moveSpeed;
			obj->SetPosition(PlayerPos);

			XMFLOAT3 rotation = obj->GetRotation();
			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}
			obj->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if ((inputD) || PadInputRIGHT) {

			XMFLOAT3 PlayerPos = obj->GetPosition();
			PlayerPos.x = PlayerPos.x + moveSpeed;
			obj->SetPosition(PlayerPos);

			XMFLOAT3 rotation = obj->GetRotation();
			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}
			obj->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if (inputQ) {

			XMFLOAT3 PlayerPos = obj->GetPosition();
			PlayerPos.y = PlayerPos.y + moveSpeed;
			obj->SetPosition(PlayerPos);
		}

		if (inputZ) {

			XMFLOAT3 PlayerPos = obj->GetPosition();
			PlayerPos.y = PlayerPos.y - moveSpeed;
			obj->SetPosition(PlayerPos);
		}

	}
	if (TriggerR) {//���Z�b�g
		obj->SetPosition({ 0,40,-170 });
		obj->SetRotation({ 0,0,0 });
	}

	if (inputUp || inputDown || inputRight || inputLeft)
	{
		XMFLOAT3 Rot = obj->GetRotation();
		if (inputUp) {
			Rot.x -= 1;
		}
		if (inputDown) {
			Rot.x += 1;
		}
		if (inputRight) {
			Rot.y += 1;
		}
		if (inputLeft) {
			Rot.y -= 1;
		}
		obj->SetRotation(Rot);
	}

	//------------------���v���C���[�ړ����p��

	//������]
	if (inputL)
	{
		XMFLOAT3 rotation = obj->GetRotation();
		rotation.y++;
		obj->SetRotation(rotation);
	}
	if (inputJ)
	{
		XMFLOAT3 rotation = obj->GetRotation();
		rotation.y--;
		obj->SetRotation(rotation);
	}
	if (inputI)
	{
		XMFLOAT3 rotation = obj->GetRotation();
		rotation.x--;
		obj->SetRotation(rotation);
	}
	if (inputK)
	{
		XMFLOAT3 rotation = obj->GetRotation();
		rotation.x++;
		obj->SetRotation(rotation);
	}

	//���ˏ���
	if (alive) { Attack(); }
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	obj->Update();

}

void Player::Draw()
{
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive)
	{
		obj->Draw();
	}
}
