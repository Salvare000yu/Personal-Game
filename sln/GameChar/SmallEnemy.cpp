#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include "GameSound.h"

#include <DirectXMath.h>
#include <random>

using namespace DirectX;

void SmallEnemy::Attack()
{
	//�L�[���͎g��
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);

	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.1f);

	//�e����
	XMFLOAT3 sePos = obj->GetPosition();
	//�e����
	std::unique_ptr<SmallEnemyBullet> madeBullet = std::make_unique<SmallEnemyBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet->Initialize();
	madeBullet->SetModel(seBulModel);
	madeBullet->SetPosition(sePos);

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
	obj->SetScale({ 6.0f, 6.0f, 6.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });
	//�ꏊ

	//SEneRandX = float(rand() % 60);
	//float SEneRandX = posXNum(random);
	//float SEneRandY = posYNum(random);
	//obj->SetPosition({ SEneRandX,SEneRandY,1000 });

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

	XMFLOAT3 sePos = obj->GetPosition();
	//�o��͉�����������Ă���
	if (isSeApproach == true) {
		if (obj->GetPosition().z > shotTag->GetPosition().z + PosZMax) {
			sePos.z -= 6;
		}
		else {
			RetireFrame--;//�؍ݎ���
			if (RetireFrame < 0) {
				//�J���J�n
				isRetire = true;
				isSeApproach = false;
			}
		}
	}
	
	if (isRetire == true) {
		//���@���E�ɂ��邩���ɂ��邩�łǂ���ɎJ���邩�ς��
		if (obj->GetPosition().x < shotTag->GetPosition().x) {
			retirePat_=RetirePat::Left;
			//���p�^�[����
			isRetire = false;
		}
		else {
			retirePat_=RetirePat::Right;
			//�E�p�^�[����
			isRetire = false;
		}
	}
	if (retirePat_ == RetirePat::Right) {
		sePos.x += 3;
	}
	if (retirePat_ == RetirePat::Left) {
		sePos.x -= 3;
	}

	obj->SetPosition(sePos);

	//���˃J�E���g���f�N�������g
	AtkCount--;

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	//������������
	if (AtkCount == 0) {
		//�������̂ݔ���
		if (alive) {
			Attack();
		}
		//�ĂуJ�E���g�ł���悤�ɏ�����
		AtkCount = AtkInterval;
	}

	//�e�X�V
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {

		//���̎��̃^�[�Q�b�g���W
		//��x����
		if (bullet->ShotTagMomOnlyFlag == true) {
			bullet->ShotTagMoment = shotTag->GetPosition();
			bullet->ShotTagMomOnlyFlag = false;
		}

		bullet->Update();

		bullet->Nowframe++;
		if (bullet->GetPosOnlyFlag == true)
		{
			//�ŏ��̈ʒu
			bullet->sePosMoment = obj->GetPosition();
			bullet->GetPosOnlyFlag = false;
		}
		//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
		// //��Γ�����
		////MoveSp.x = (shotTag->GetPosition().x - sePosMoment.x);
		////MoveSp.y = (shotTag->GetPosition().y - sePosMoment.y);
		////MoveSp.z = (shotTag->GetPosition().z - sePosMoment.z);
		bullet->MoveSp.x = (bullet->ShotTagMoment.x - bullet->sePosMoment.x);
		bullet->MoveSp.y = (bullet->ShotTagMoment.y - bullet->sePosMoment.y);
		bullet->MoveSp.z = (bullet->ShotTagMoment.z - bullet->sePosMoment.z);

		//XMVECTOR�ɕϊ�����xmvecMoveSp�ɂ����
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&bullet->MoveSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// �傫����C�Ӓl��(���x)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 10.f);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&bullet->MoveSp, xmvecMoveSp);

		//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
		bullet->NowPos.x = bullet->sePosMoment.x + bullet->MoveSp.x * bullet->Nowframe;
		bullet->NowPos.y = bullet->sePosMoment.y + bullet->MoveSp.y * bullet->Nowframe;
		bullet->NowPos.z = bullet->sePosMoment.z + bullet->MoveSp.z * bullet->Nowframe;

		bullet->SetPosition(bullet->NowPos);//���̎��̈ʒu
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
