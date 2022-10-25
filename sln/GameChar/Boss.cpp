#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"

#include <DirectXMath.h>

void Boss::ApproachInit()
{
	//�U���p�J�E���g���������ĊԊu�������Έ�U�҂��Ă��甭�ˉ\
	AtkCount = AtkInterval;
	DiffusionAtkCount = DiffusionAtkInterval;
}

void Boss::Attack()
{
	//�L�[���͎g��
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//�e����
	XMFLOAT3 position = obj->GetPosition();
	//�e����
	std::unique_ptr<BossBullet> madeBullet = std::make_unique<BossBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet->Initialize();
	madeBullet->SetModel(eBulModel);
	madeBullet->SetPosition(position);

	//�e�o�^
	bullets_.push_back(std::move(madeBullet));
}

void Boss::Approach()
{
	//---�ˌ�---//
	//���˃J�E���g���f�N�������g
	AtkCount--;
	//������������
	if (AtkCount == 0) {
		//�ˌ����A�������̂ݔ���
		if (alive) { Attack(); }
		//�ĂуJ�E���g�ł���悤�ɏ�����
		AtkCount = AtkInterval;
	}

	//�G�̈ړ�
	XMFLOAT3 position = obj->GetPosition();
	position.z -= ApproachSp;
	position.y += ApproachSp;
	position.x += 3.f * sinf(time * 3.14159265358f);
	obj->SetPosition(position);

	//������x�߂Â����痣���
	if (position.z == ApproachLim) {
		actionPattern_ = ActionPattern::Leave;
	}
}

void Boss::Leave()
{
	//---���---//

	//���˃J�E���g���f�N�������g
	DiffusionAtkCount--;
	//������������
	if (DiffusionAtkCount == 0) {
		//�������̂ݔ���
		if (alive) { DiffusionAttack(); }
		//�ĂуJ�E���g�ł���悤�ɏ�����
		DiffusionAtkCount = DiffusionAtkInterval;
	}

	//---���---//
	XMFLOAT3 positionBack = obj->GetPosition();
	positionBack.z += ApproachSp;
	positionBack.y -= ApproachSp;
	obj->SetPosition(positionBack);

	//������x���ꂽ��߂Â��Ă���
	if (positionBack.z == LeaveLim) {
		actionPattern_ = ActionPattern::Approach;
	}
}

void Boss::DiffusionAttack()
{
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//�e����
	XMFLOAT3 position = obj->GetPosition();
	//�e����
	std::unique_ptr<BossBullet> madeBullet = std::make_unique<BossBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet->Initialize();
	madeBullet->SetModel(eBulModel);
	madeBullet->SetPosition(position);

	// velocity���Z�o
	DirectX::XMVECTOR vecvelocity = DirectX::XMVectorSet(0, 0, 3, 0);
	XMFLOAT3 xmfloat3velocity;
	XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

	madeBullet->SetVelocity(xmfloat3velocity);

	//�e�o�^
	bullets_.push_back(std::move(madeBullet));
}

void Boss::Initialize()
{

	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 20.0f, 20.0f, 20.0f });
	//�ꏊ
	obj->SetPosition({ -100,50,50 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");

	//�߂Â��p�^�[��������
	ApproachInit();
}

void Boss::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;

	//if (input3) {
	//	XMFLOAT3 position = obj->GetPosition();
	//	position.z += 5;
	//	obj->SetPosition(position);
	//}

		//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return !bullet->GetAlive();
		});

	//�����̉�]
	for (int i = 0; i < 1; i++)
	{
		frame += 1.f;

		//XMFLOAT3 rotation = obj->GetRotation();
		//rotation.y += 0.7f;
		//rotation.x += 0.4f;
		//obj->SetRotation({ rotation });

		XMFLOAT3 position = obj->GetPosition();
		//position.x += 5.f * sin(time * 3.14159265358f);
		obj->SetPosition(position);
	}
	//----------------------------------------------���֐�������{�P
	//switch (actionPattern_) {
	//case ActionPattern::Approach://�߂Â��p�^�[��
	//default:
	//	//---�ˌ�---//
	//	//���˃J�E���g���f�N�������g
	//	AtkCount--;
	//	//������������
	//	if (AtkCount == 0) {
	//		//�ˌ����A�������̂ݔ���
	//		if (alive) { Attack(); }
	//		//�ĂуJ�E���g�ł���悤�ɏ�����
	//		AtkCount = AtkInterval;
	//	}

	//	//�G�̈ړ�
	//	XMFLOAT3 position = obj->GetPosition();
	//	position.z -= ApproachSp;
	//	position.y += ApproachSp;
	//	position.x += 3.f * sinf(time * 3.14159265358f);
	//	obj->SetPosition(position);

	//	//������x�߂Â����痣���
	//	if (position.z == ApproachLim) {
	//		actionPattern_ = ActionPattern::Leave;
	//	}
	//	break;
	//case ActionPattern::Leave://��ރp�^�[��
	//	//���˃J�E���g���f�N�������g
	//	DiffusionAtkCount--;
	//	//������������
	//	if (DiffusionAtkCount == 0) {
	//		//�������̂ݔ���
	//		if (alive) { DiffusionAttack(); }
	//		//�ĂуJ�E���g�ł���悤�ɏ�����
	//		DiffusionAtkCount = DiffusionAtkInterval;
	//	}

	//	//---���---//
	//	XMFLOAT3 positionBack = obj->GetPosition();
	//	positionBack.z += ApproachSp;
	//	positionBack.y -= ApproachSp;
	//	obj->SetPosition(positionBack);

	//	//������x���ꂽ��߂Â��Ă���
	//	if (positionBack.z == LeaveLim) {
	//		actionPattern_ = ActionPattern::Approach;
	//	}
	//	break;
	//----------------------------------------------���֐�������{�P

	//�����o�֐��|�C���^�Ή������I��
	if (actionPattern_ == ActionPattern::Approach) { pFunc = &Boss::Approach; }
	if (actionPattern_ == ActionPattern::Leave) { pFunc = &Boss::Leave; }
	//if (PauseNowSelect == 2) { pFunc = &GamePlayScene::PauseGoTitle; }
	//�����o�֐��|�C���^�Ăяo��
	(this->*pFunc)();

	//�e�X�V
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}

	obj->Update();
}

void Boss::Draw()
{
	//�e
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive) {
		obj->Draw();
	}
}
