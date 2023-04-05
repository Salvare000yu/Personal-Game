#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include "GameSound.h"

#include <DirectXMath.h>
#include <random>

using namespace DirectX;

void SmallEnemy::Attack()
{
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.1f);

	//�e����
	XMFLOAT3 sePos = obj->GetPosition();
	//�e����
	std::unique_ptr<SmallEnemyBullet> madeBullet = std::make_unique<SmallEnemyBullet>();
	madeBullet->Initialize();
	madeBullet->SetModel(seBulModel);
	madeBullet->SetPosition(sePos);

	//�e�o�^
	bullets_.push_back(std::move(madeBullet));
}

void SmallEnemy::Initialize()
{
	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 6.0f, 6.0f, 6.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });

	AtkCount = AtkInterval;
}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<SmallEnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});

	XMFLOAT3 sePos = obj->GetPosition();
	//�o��͉�����������Ă���
	if (isSeApproach) {
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

	if (isRetire) {
		//���@���E�ɂ��邩���ɂ��邩�łǂ���ɎJ���邩�ς��
		if (obj->GetPosition().x < shotTag->GetPosition().x) {
			retirePat_ = RetirePat::Left;
			//���p�^�[����
			isRetire = false;
		}
		else {
			retirePat_ = RetirePat::Right;
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

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { alive = false; }

	//������������
	if (--AtkCount == 0) {
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
		if (bullet->ShotTagMomOnlyFlag) {
			bullet->ShotTagMoment = shotTag->GetPosition();
			bullet->ShotTagMomOnlyFlag = false;
		}

		bullet->Update();

		bullet->Nowframe++;
		if (bullet->GetPosOnlyFlag)
		{
			//�ŏ��̈ʒu
			bullet->sePosMoment = obj->GetPosition();
			bullet->GetPosOnlyFlag = false;
		}
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