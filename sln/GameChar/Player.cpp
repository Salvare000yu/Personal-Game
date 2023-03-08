#include "Player.h"
//#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "CharParameters.h"
#include "ParticleManager.h"

#include <DirectXMath.h>

using namespace DirectX;

//Player* Player::GetInstance()
//{
//	static Player instance;
//	return &instance;
//}

void Player::Attack()
{
	//�L�[���͎g��
	Input* input = Input::GetInstance();

	CharParameters* charParameters = CharParameters::GetInstance();

	//triggerkey
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//�p�b�h�������u�Ԃ̂�
	const bool PadTriggerRB = input->TriggerButton(static_cast<int>(Button::RB));

	const bool TriggerMouseLEFT = input->TriggerMouse(0);

	//Input
	const bool InputSPACE = input->PushKey(DIK_SPACE);
	const bool PadInputRB = input->PushButton(static_cast<int>(Button::RB));
	const bool InputMouseLEFT = input->PushMouse(0);

	//�e��
	const int pBulVel = 60;

	//����������
	if ((InputSPACE || PadInputRB || InputMouseLEFT) && AttackIntervalFlag == false)
	{
		XMFLOAT3 PlayerPos = obj->GetPosition();
		//�e����
		std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
		//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
		madeBullet->Initialize();
		madeBullet->SetModel(pBulModel);
		madeBullet->SetPosition(PlayerPos);

		// velocity���Z�o �e���ˑ��xz
		DirectX::XMVECTOR vecvelocity = XMVectorSet(0, 0, pBulVel, 0);
		XMFLOAT3 xmfloat3velocity;
		XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

		madeBullet->SetVelocity(xmfloat3velocity);

		// �����Đ� �炵�����Ƃ�
		GameSound::GetInstance()->PlayWave("shot.wav", 0.1f);

		//�e�o�^
		bullets_.push_back(std::move(madeBullet));

		//input->PadVibrationDef();

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
		}
		else { AtkInterval_ = AtkInterval; }
	}

}

void Player::Move()
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

	XMFLOAT3 rotation = obj->GetRotation();

	//������]
	if (inputL)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.y++;
		obj->SetRotation(rotation);
	}
	if (inputJ)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.y--;
		obj->SetRotation(rotation);
	}
	if (inputI)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.x--;
		obj->SetRotation(rotation);
	}
	if (inputK)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.x++;
		obj->SetRotation(rotation);
	}
}

void Player::Shake() {

	CharParameters* charParameters = CharParameters::GetInstance();
	Input* input = Input::GetInstance();

	if (--pShakeTimer_ >= 0) {// 0�܂Ō��炷			
		//DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

		input->PadVibration();

		//pos�h�炷
		XMFLOAT3 pos = obj->GetPosition();
		randShakeNow = 1 + 1;//a~b
		pos.x = pos.x + rand() % randShakeNow - 0.5f;//a~b�܂ł�randShakeNow�̍ő�l���甼���������ĕ��̐����܂ނ悤��
		pos.y = pos.y + rand() % randShakeNow - 0.5f;
		obj->SetPosition(pos);

		if (pShakeTimer_ <= 0) {
			input->PadVibrationDef();
		}

	}
	else {
		pShakeTimer_ = pShakeTime;
		charParameters->SetispDam(false);
	}

}
void Player::PlayerDeath()
{
	Nowframe++;
	ParticleFrame++;
	PartTimeInterval = ParticleFrame / 40;

	XMFLOAT3 position = obj->GetPosition();

	if (GetPosFlag == true)
	{
		//�ŏ��̈ʒu
		pPosDeath = obj->GetPosition();
		GetPosFlag = false;
	}

	//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
	MoveSp.x = (pPosDeath.x - pPosDeath.x) / NecesFrame;//�����̎w����W�͎��@�ŏ���X���W�ɂ��Đ^���ɗ�����悤�ɂ���
	MoveSp.y = (TargetPos.y - pPosDeath.y) / NecesFrame;
	MoveSp.z = (pPosDeath.z - pPosDeath.z) / NecesFrame;//���s�������炱������������
	//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
	NowPos.x = pPosDeath.x + MoveSp.x * Nowframe;
	NowPos.y = pPosDeath.y + MoveSp.y * Nowframe;
	NowPos.z = pPosDeath.z + MoveSp.z * Nowframe;

	obj->SetPosition(NowPos);//���̎��̈ʒu

	//��莞�Ԃ��ƂɃp�[�e�B�N��
	if (PartTimeInterval == 1) {
		ExplosionFlag = true;
		// �����Đ� �炵�����Ƃ�
		GameSound::GetInstance()->PlayWave("destruction1.wav", 0.2f);
		ParticleManager::GetInstance()->CreateParticle(NowPos, 50, 30, 10);
		PartTimeInterval = 0;
		ParticleFrame = 0;
	}

	if (ExplosionFlag == true) {
		Shake();
		if (pShakeTimer_ <= 0) {
			ExplosionFlag = false;
		}
	}

	//��]
	XMFLOAT3 pRot = obj->GetRotation();
	pRot.z += pDeathRot;
	obj->SetRotation(pRot);

	if (obj->GetPosition().y < TargetPos.y)
	{
		//�ڕW���B�Ŏ��S�t���O�I��
		PlayerDeathFlag = true;
	}
}

void Player::FiringLine()
{

	//���낢�됶��
	firingline_.reset(new PlayerFireLine());
	//���낢��L����������
	firingline_ = std::make_unique<PlayerFireLine>();
	firingline_->Initialize();
	firingline_->SetModel(pFiringLine);

	XMFLOAT3 PlayerPos = obj->GetPosition();
	firingline_->SetPosition({ PlayerPos.x,PlayerPos.y,PlayerPos.z });

	XMFLOAT3 PlayerRot = obj->GetRotation();
	firingline_->SetRotation(PlayerRot);

	//firingline_->SetScale({ 0.5f,0.5f,10.f });
}

void Player::Initialize()
{
	//��`�Ƃ��������Ă�����

	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	//�ꏊ
	obj->SetPosition({ 0,70,-250 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("shot.wav");

}

void Player::Update()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	float pHp = charParameters->GetNowpHp();
	if (pHp <= 0) {
		isPHpLessThan0 = true;//���@�̗͂�0��������Ă���
	}

	//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
		});

	//�U�����Ă悢�󋵂Ȃ�
	if (pAtkPossibleFlag) {
		//���ˏ��� �����Ă�Hp0�ȏ�Ȃ�
		if (alive && isPHpLessThan0 == false) {
			Attack();
		}
	}

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�����Ă�Hp�O�����傤�Ȃ�
	if (alive && isPHpLessThan0 == false) {
		FiringLine();
		if (pAtkPossibleFlag) {//�U�����Ă����Ƃ��Ȃ�
			firingline_->Update();
			Move();
		}
	}
	else {
		//0�ȉ��Ȃ�����
		PlayerDeath();
	}

	//���@�������Ă��ԂɂȂ�����
	if (charParameters->GetispDam() == true) {
		//HP0�ȉ��Ȃ���Ȃ��悤�Ɂ@���S���o����Ă�����
		if (charParameters->GetNowpHp() > 0)
		{
			Shake();
		}
	}

	//���@�̒e�З͂��}�C�i�X�l�ɂȂ�Ȃ�
	//0�ȉ��Ȃ�P�ɂ���
	if (pBulPower <= 0) {
		pBulPower = 1;
	}

	obj->Update();

}

void Player::Draw()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive)
	{
		obj->Draw();
	}

	if (alive && isPHpLessThan0 == false) {
		if (pAtkPossibleFlag) {//�U�����Ă����Ƃ��Ȃ�
			firingline_->Draw();
		}
	}
}
