#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include "GameSound.h"

#include <DirectXMath.h>
#include <random>

void SmallEnemy::Attack()
{
	//�L�[���͎g��
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);

	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.1f);

	//�e����
	XMFLOAT3 position = obj->GetPosition();
	//�e����
	std::unique_ptr<SmallEnemyBullet> madeBullet = std::make_unique<SmallEnemyBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet->Initialize();
	madeBullet->SetModel(seBulModel);
	madeBullet->SetPosition(position);

	//�e�o�^
	bullets_.push_back(std::move(madeBullet));
}

void SmallEnemy::Initialize()
{
	//std::srand((unsigned)std::time(nullptr));
	const float posXMin = -200.f;
	const float posXMax = 200.f;
	const float posYMin = 10.f;
	const float posYMax = 70.f;

	std::random_device seed;
	std::mt19937 random(seed());
	std::uniform_int_distribution<> posXNum(posXMin, posXMax);
	std::uniform_int_distribution<> posYNum(posYMin, posYMax);

	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });
	//�ꏊ

	//SEneRandX = float(rand() % 60);
	float SEneRandX = posXNum(random);
	float SEneRandY = posYNum(random);
	obj->SetPosition({ SEneRandX,SEneRandY,1000 });

	AtkCount = AtkInterval;

}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	//srand((unsigned int)timeGetTime());
	//SEneRandX = SEneMinX + (int)(rand() * (SEneMaxX - SEneMinX + 1) / (1 + RAND_MAX));
	//XMFLOAT3 position = obj->GetPosition();
	//position.x = SEneRandX;
	//obj->SetPosition(position);

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}
			//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<SmallEnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});

	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 smEnemPos = obj->GetPosition();
		smEnemPos.z -= 4;
		obj->SetPosition(smEnemPos);

		//���˃J�E���g���f�N�������g
		AtkCount--;
	}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	//������������
	if (AtkCount == 0) {
		//�������̂ݔ���
		if (alive) { Attack(); }
		//�ĂуJ�E���g�ł���悤�ɏ�����
		AtkCount = AtkInterval;
	}

	//�e�X�V
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	obj->Update();
}

void SmallEnemy::Draw()
{
	//�e�`��
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive) {
		obj->Draw();
	}
}
