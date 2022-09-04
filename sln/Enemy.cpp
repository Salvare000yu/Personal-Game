#include "Enemy.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"

#include <DirectXMath.h>

void Enemy::ApproachInit()
{
	//�U���p�J�E���g���������ĊԊu�������Έ�U�҂��Ă��甭�ˉ\
	AtkCount = AtkInterval;
}

Enemy* Enemy::GetInstance()
{
	static Enemy instance;

	return &instance;
}

void Enemy::OnCollision()
{
}

void Enemy::Attack()
{
	//�L�[���͎g��
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.5);

	//�e����
	XMFLOAT3 position = obj_enemy->GetPosition();
	//�e����
	std::unique_ptr<EnemyBullet> madeBullet = std::make_unique<EnemyBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet->Initialize({ position });

	//�e�o�^
	bullets_.push_back(std::move(madeBullet));
}

void Enemy::Initialize()
{

	//���ł�ǂݍ���
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
	//���
	obj_enemy.reset(Object3d::Create());
	//�Z�b�g
	obj_enemy->SetModel(mod_enemy.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_enemy->SetScale({ 20.0f, 20.0f, 20.0f });
	//�ꏊ
	obj_enemy->SetPosition({ -100,50,50 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");

	//�߂Â��p�^�[��������
	ApproachInit();
}

void Enemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}

		//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsVanish();
		});

	//�����̉�]
	for (int i = 0; i < 1; i++)
	{
		frame += 1.f;

		XMFLOAT3 rotation = obj_enemy->GetRotation();
		rotation.y += 0.7f;
		rotation.x += 0.4f;
		obj_enemy->SetRotation({ rotation });

		XMFLOAT3 position = obj_enemy->GetPosition();
		//position.x += 5.f * sin(time * 3.14159265358f);
		obj_enemy->SetPosition(position);
	}
	//----------------------------------------------���֐�������{�P
	switch (actionPattern_) {
	case ActionPattern::Approach://�߂Â��p�^�[��
	default:
		//---�ˌ�---//
		//���˃J�E���g���f�N�������g
		AtkCount--;
		//������������
		if (AtkCount == 0) {
			//�ˌ����̂ݔ���
			Attack();
			//�ĂуJ�E���g�ł���悤�ɏ�����
			AtkCount = AtkInterval;
		}

		//�e�̈ړ�
		XMFLOAT3 position = obj_enemy->GetPosition();
		position.z -= ApproachSp;
		position.y += ApproachSp;
		position.x += 3.f * sin(time * 3.14159265358f);
		obj_enemy->SetPosition(position);

		//������x�߂Â�����L���߂��ė����
		if (position.z == ApproachLim) {
			actionPattern_ = ActionPattern::Leave;
		}
		break;
	case ActionPattern::Leave://��ރp�^�[��
		//---���---//
		XMFLOAT3 positionBack = obj_enemy->GetPosition();
		positionBack.z += ApproachSp;
		positionBack.y -= ApproachSp;
		obj_enemy->SetPosition(positionBack);

		//������x���ꂽ��߂Â��Ă���
		if (positionBack.z == LeaveLim) {
			actionPattern_ = ActionPattern::Approach;
		}
		break;
	}
	//----------------------------------------------���֐�������{�P

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	obj_enemy->Update();
}

void Enemy::Draw()
{
	//�e
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	obj_enemy->Draw();
}
