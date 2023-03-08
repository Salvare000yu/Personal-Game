#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "CharParameters.h"

#include <DirectXMath.h>

#include "GamePlayScene.h"

void Boss::BossAppear()
{

	CharParameters* charParams = CharParameters::GetInstance();

	XMFLOAT3 pos = obj->GetPosition();

	//�ړ������m�F������
	if (charParams->pNextPlaceGoFlag == false) {
		ActionStartPos = obj->GetPosition();//�U���Ɉڂ�Ƃ��̍��W�擾Leave�ŗ������E�l�Ŏg��
		actionPattern_ = ActionPattern::Approach;
	}

	pos.y += 2.f * sinf(time * 3.14159265358f);

	obj->SetPosition(pos);
}

void Boss::ApproachInit()
{
	//�U���p�J�E���g���������ĊԊu�������Έ�U�҂��Ă��甭�ˉ\
	AtkCount = AtkInterval;
	DiffusionAtkCount = DiffusionAtkInterval;
	Circular_AtkCount = Circular_AtkInterval;
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
	position.z -= ApproachSpZ;
	position.y += ApproachSpY;
	position.x += 7.f * sinf(time * 3.14159265358f);
	obj->SetPosition(position);

	////�߂Â������͎��@�̏ꏊ�Ɏ��@�Ɨ��������������Ԃ��J����
	//const int SpaceDistance = 200;
	//ApproachLim = shotTag->GetPosition().z + SpaceDistance;

	ApproachCount--;

	//������x�߂Â����痣���
	if (ApproachCount == 0) {
		ApproachCount = ApproachCountDef;
		ChangeVerticalCount++;//�c�U�����邽�߂̃J�E���g�i�߂�
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
		if (alive) {
			if (even_odd_NumFlag == true)//��e
			{
				DiffusionAttack();
			}
			else { DiffusionAttackEavenNumber(); }
		}
		//�ĂуJ�E���g�ł���悤�ɏ�����
		DiffusionAtkCount = DiffusionAtkInterval;
	}

	//---���---//
	XMFLOAT3 positionBack = obj->GetPosition();
	positionBack.z += LeaveSpZ;
	positionBack.y -= LeaveSpY;
	obj->SetPosition(positionBack);

	//����鐧���͎��@�̏ꏊ�Ɏ��@�Ɨ��������������Ԃ��J����
	const int SpaceDistance = 400;
	LeaveLim = shotTag->GetPosition().z + SpaceDistance;

	//������x���ꂽ��߂Â��Ă���
	if (positionBack.z > ActionStartPos.z && positionBack.y < ActionStartPos.y) {
		if (even_odd_NumFlag == true) { even_odd_NumFlag = false; }
		else { even_odd_NumFlag = true; }

		if (ChangeVerticalCount == ChangeVerticalNeces) {//�c�U���J�E���g�����ɒB������
			actionPattern_ = ActionPattern::Vertical;//�c�U��
		}
		else {
			actionPattern_ = ActionPattern::Approach;
		}

	}
}
void Boss::Vertical()
{
	XMFLOAT3 position = obj->GetPosition();

	switch (verticalPattern_) {

	case VerticalPattern::def://�f�t�H���g
		verticalPattern_ = VerticalPattern::StartVertical;//�㏸�J�n
		break;

	case VerticalPattern::StartVertical://�܂��͏㏸
		position.y += StartVerticalVal;
		StartVerticalVal++;

		if (position.y > DownStartPosY) {//��蒴�����画��؂��Ă���҂�
			StartVerticalVal = StartVerticalValDef;//�ŏ��̏㏸�l�߂�
			verticalPattern_ = VerticalPattern::Wait;//�㏸�J�n
		}
		break;

	case VerticalPattern::Wait:
		doCollision = false;//Wait���͉�ʊO�Ȃ̂œ����蔻����Ȃ�
		VerticalWaitCount--;//�҂��J�E���g���炷

		if (VerticalWaitCount == 0) {
			VerticalWaitCount = VerticalWaitCountDef/2;//����ȍ~�̓f�t�H/2�ɃJ�E���g�߂�
			doCollision = true;//�����蔻��I���ɂ��Ă��玟�s��

			if (VerticalLoopCount != 0) {//UpDown���[�v�񐔂�0�łȂ���΁A�p��
				if (NextDown == true) {//�����~�Ȃ�
					NextDown = false;//�߂��Ă���
					verticalPattern_ = VerticalPattern::Down;//���ړ�
				}
				if (NextUp == true) {//���㏸�Ȃ�
					NextUp = false;//�߂��Ă���
					verticalPattern_ = VerticalPattern::Up;//��ړ�
				}
			}
			else {//��[�Ղ��w�萔���������i�J�E���g0�Łj
				//���̓����߂�
				NextUp = false;
				NextDown = true;//�ŏ��͉��~
				VerticalLoopCount = VerticalLoopCountDef;
				verticalPattern_ = VerticalPattern::Reverse;//�߂�p�^�[����
			}
		}
		break;

	case VerticalPattern::Down:
		if (VerticalStartPosFlag == false) {//�ŏ��ɊJ�n�ʒu���߂�
			UpDownPos = { UpDownPos.x, DownStartPosY,position.z };//Y�͊J�n�ʒu�@Z�͌������ʒu
			position = UpDownPos;//���̈ʒu�����ɉ�����J�n�ʒu�ɂ���
			VerticalStartPosFlag = true;//�ŏ��̊J�n�ʒu�����ߏI�����
		}
		position.y -= VerticalSp;//�ړ�
		if (position.y <= UpStartPosY) {//�㏸�J�n�ʒu��Y�܂ł�����
			UpDownPos.x += NextMoveX;//���̈ړ����̂��߂�X�����Ƃ�
			//���҂����ԋ���łˁ@Wait���͔���؂�
			VerticalStartPosFlag = false;//�J�n�ʒu����t���O�߂�
			NextUp = true;//���㏸
			VerticalLoopCount--;//Down������Ƃ��Ƀf�N�������g
			verticalPattern_ = VerticalPattern::Wait;//�܂��Ă�
		}

		//���˃J�E���g���f�N�������g
		AtkCount--;
		//������������
		if (AtkCount == 0) {
			//�ˌ����A�������̂ݔ���
			if (alive) { Attack(); }//�ǔ��e
			//�ĂуJ�E���g�ł���悤�ɏ�����
			AtkCount = Vertical_AtkInterval;
		}
		break;

	case VerticalPattern::Up:
		if (VerticalStartPosFlag == false) {//�ŏ��ɊJ�n�ʒu���߂�
			UpDownPos = { UpDownPos.x, UpStartPosY,position.z };//Y�͊J�n�ʒu�@Z�͌������ʒu
			position = UpDownPos;//���̈ʒu�����ɉ�����J�n�ʒu�ɂ���
			VerticalStartPosFlag = true;//�ŏ��̊J�n�ʒu�����ߏI�����
		}
		position.y += VerticalSp;//�ړ�
		if (position.y >= DownStartPosY) {//�㏸�J�n�ʒu��Y�܂ł�����
			UpDownPos.x += NextMoveX;//���̈ړ����̂��߂�X�����Ƃ�
			//���҂����ԋ���łˁ@Wait���͔���؂�
			VerticalStartPosFlag = false;//�J�n�ʒu����t���O�߂�
			NextDown = true;//���㏸
			verticalPattern_ = VerticalPattern::Wait;//�܂��Ă���
		}

		AtkCount--;
		//������������
		if (AtkCount == 0) {
			//�ˌ����A�������̂ݔ���
			if (alive) { StraightAttack(); }
			//�ĂуJ�E���g�ł���悤�ɏ�����
			AtkCount = Vertical_AtkInterval;
		}
		break;

	case VerticalPattern::Reverse:
		if (ReverseStartPosFlag == false) {//��������߂�n�߂�
			position = { ReverseStartPos.x,position.y,position.z };//X�AY�͍Ō�̍����̂܂܁AZ
			ReverseStartPosFlag = true;
		}
		position.y += VerticalSp;

		if (position.y>= ReverseStartPos.y) {//�{�X��J�n����Y���W���B�ōs���I���
			ReverseStartPosFlag = false;
			verticalPattern_ = VerticalPattern::EndVertical;//�c�p�^�[���̍s���I������O�ɂ�邱��
		}
		break;

	case VerticalPattern::EndVertical:
		UpDownPos = UpDownPosDef;//��Ɖ��p�̍��W�����ɖ߂�
		VerticalWaitCount = VerticalWaitCountDef;//�҂��J�E���g�f�t�H���g�ɖ߂�
		ChangeVerticalCount = ChangeVerticalCountDef;//�c�U�����邽�߂ɕK�v�ȃJ�E���g���f�t�H�ɖ߂�
		verticalPattern_ = VerticalPattern::def;//�܂��g����悤�Ƀf�t�H�ɖ߂��Ƃ�
		actionPattern_ = ActionPattern::Approach;//��{�̍s���p�^�[����Approach�ɖ߂�
		break;

	}

	obj->SetPosition(position);
}

void Boss::HpHalfPatStart()
{

	CharParameters* charParams = CharParameters::GetInstance();

	Nowframe++;

	//���@�̏ꏊ
	XMFLOAT3 pPos = shotTag->GetPosition();
	//�w����W�Ŏ��@��Z����ǂꂭ�炢�Ԃ������邩
	const int SpaceDistance = 500;

	if (GetPosOnlyFlag == true)
	{
		//�ŏ��̈ʒu
		HpHalfMomentPos = obj->GetPosition();
		GetPosOnlyFlag = false;

		//�w����W�ǂ���
		TargetHpHalfPos = { 0,0,pPos.z + SpaceDistance };

		//�h��͏オ��
		float Defence = charParams->GetBossDefense();
		Defence += 30;
		charParams->SetBossDefense(Defence);
	}

	//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
	MoveSp.x = (pPos.x - HpHalfMomentPos.x) / NecesHpHalfFrame;
	MoveSp.y = (TargetHpHalfPos.y - HpHalfMomentPos.y) / NecesHpHalfFrame;
	MoveSp.z = (TargetHpHalfPos.z - HpHalfMomentPos.z) / NecesHpHalfFrame;
	//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
	NowPos.x = HpHalfMomentPos.x + MoveSp.x * Nowframe;
	NowPos.y = HpHalfMomentPos.y + MoveSp.y * Nowframe;
	NowPos.z = HpHalfMomentPos.z + MoveSp.z * Nowframe;

	obj->SetPosition({ NowPos });

	//���˃J�E���g���f�N�������g
	DiffusionAtkCount--;
	if (DiffusionAtkCount == 0) {
		//�������̂ݔ���
		if (alive) {
			if (even_odd_NumFlag == true)//��e
			{
				DiffusionAttack();
			}
			else { DiffusionAttackEavenNumber(); }
		}
		//�ĂуJ�E���g�ł���悤�ɏ�����
		DiffusionAtkCount = DiffusionAtkInterval;
	}
	//���˃J�E���g���f�N�������g
	AtkCount--;
	//������������
	if (AtkCount == 0) {
		//�ˌ����A�������̂ݔ���
		if (alive) { Attack(); }//�ǔ��e
		//�ĂуJ�E���g�ł���悤�ɏ�����
		AtkCount = AtkInterval;
	}

	if (Nowframe == NecesHpHalfFrame) {
		Nowframe = NowframeDef;
		GetPosOnlyFlag = true;
		HpHalfMomentPos = {};
		MoveSp = {};
		NowPos = {};

		isHpHalfPattern = true;//���̏����S���I����������������Ȃ�
		actionPattern_ = ActionPattern::CircularMotionMove;
	}
}
void Boss::CircularMotionMove()
{
	const float ApproachZ = 0.15f;//�߂Â��l
	const float CircularY = 40;//�ǂ̍����ŉ�邩

	//�G�̈ړ�
	XMFLOAT3 position = obj->GetPosition();
	//�ʓx�@
	HpHalf_rad = HpHalf_Angle * 3.1415926535f / 180.0f;

	//�~�̈ʒu���O�p�֐��ł���
	addX = cosf(HpHalf_rad) * HpHalf_Length;
	addY = sinf(HpHalf_rad) * HpHalf_Length;

	if (GetPosOnlyFlag == true)
	{
		//�ŏ��̈ʒu
		CirclePosMem = obj->GetPosition();
		GetPosOnlyFlag = false;
	}

	XMFLOAT3 pPos = shotTag->GetPosition();
	//���S���W�Ɉړ��ʂ𑫂����l�����W��
	position.x = CirclePosMem.x + addX;
	position.y = CircularY + addY;
	position.z -= ApproachZ;//�n���ɔ����Ă���

	obj->SetPosition(position);

	HpHalf_Angle += 5.0f;//�p�x�@���₷�ƈړ������Ȃ�
	HpHalf_Length += 0.3f;//�Q�������悤�ɍL������

	//���a�����ȏ�ōs���ς���
	if (HpHalf_Length >= 150) {
		//---�~�^����
		HpHalf_Angle = HpHalf_AngleDef;
		HpHalf_rad = HpHalf_radDef;
		HpHalf_Length = HpHalf_LengthDef;
		addX = addXDef;
		addY = addYDef;
		GetPosOnlyFlag = true;
		actionPattern_ = ActionPattern::LeaveFirstPos;

	}

	//���˃J�E���g���f�N�������g
	Circular_AtkCount--;
	//������������
	if (Circular_AtkCount == 0) {
		//�ˌ����A�������̂ݔ���
		if (alive) { Attack(); }//�ǔ��e

		//���񂾂�e�̔��ˊԊu����
		Circular_AtkInterval -= 2;

		//�ĂуJ�E���g�ł���悤�ɏ�����
		Circular_AtkCount = Circular_AtkInterval;
	}
}
void Boss::LeaveFirstPos()
{

	Nowframe++;

	if (GetPosOnlyFlag == true)
	{
		//�ŏ��̈ʒu
		HpHalfMomentPos = obj->GetPosition();
		GetPosOnlyFlag = false;

		//LeaveFirstPos2��œ˂�����
		PlungeCount--;

	}
	XMFLOAT3 pPos = shotTag->GetPosition();
	//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
	MoveSp.x = (pPos.x - HpHalfMomentPos.x) / NecesLeaveFirstFrame;
	MoveSp.y = (TargetHpHalfPos.y - HpHalfMomentPos.y) / NecesLeaveFirstFrame;
	MoveSp.z = (TargetHpHalfPos.z - HpHalfMomentPos.z) / NecesLeaveFirstFrame;
	//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
	NowPos.x = HpHalfMomentPos.x + MoveSp.x * Nowframe;
	NowPos.y = HpHalfMomentPos.y + MoveSp.y * Nowframe;
	NowPos.z = HpHalfMomentPos.z + MoveSp.z * Nowframe;

	obj->SetPosition({ NowPos });

	//���˃J�E���g���f�N�������g
	AtkCount--;
	//������������
	if (AtkCount == 0) {
		//�ˌ����A�������̂ݔ���
		if (alive) { Attack(); }//�ǔ��e
		//�ĂуJ�E���g�ł���悤�ɏ�����
		AtkCount = AtkInterval_LeaveFirst;
	}

	if (Nowframe == NecesLeaveFirstFrame) {
		Nowframe = NowframeDef;
		GetPosOnlyFlag = true;
		HpHalfMomentPos = {};
		MoveSp = {};
		NowPos = {};

		//�e�̔��ˊԊu�����ɖ߂�
		Circular_AtkInterval = Circular_AtkIntervalDef;

		if (PlungeCount == 0) {//LeaveFirstPos���w��񐔂�����ˌ�
			XMFLOAT3 boPos = obj->GetPosition();
			WasPosMem = boPos;//�˂����ݍs���ֈڍs����O�ɍŌ�ɂ����ꏊ���L������
			actionPattern_ = ActionPattern::PlungeInto;
		}
		else {
			plungeIntoPattern_ = PlungeIntoPattern::Leave;
			actionPattern_ = ActionPattern::CircularMotionMove;
		}
	}
}
void Boss::PlungeInto()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	XMFLOAT3 position = obj->GetPosition();

	switch (plungeIntoPattern_)
	{

	case PlungeIntoPattern::Leave://��x�����
		position.z += LeaveVel;

		if (position.z >= LeavePos) {
			plungeIntoPattern_ = PlungeIntoPattern::Wait;
		}

		obj->SetPosition(position);

		break;

	case PlungeIntoPattern::PlungeInto://�˂�����ł���

		Nowframe++;

		//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
		PlungeSp.x = (pPosMem.x - boPosMom.x);
		PlungeSp.y = (pPosMem.y - boPosMom.y);
		PlungeSp.z = (pPosMem.z - boPosMom.z);

		//XMVECTOR�ɕϊ�����xmvecMoveSp�ɂ����
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&PlungeSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// �傫����C�Ӓl��(���x)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 50.f);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&PlungeSp, xmvecMoveSp);

		//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
		PlungeNowPos.x = boPosMom.x + PlungeSp.x * Nowframe;
		PlungeNowPos.y = boPosMom.y + PlungeSp.y * Nowframe;
		PlungeNowPos.z = boPosMom.z + PlungeSp.z * Nowframe;

		obj->SetPosition(PlungeNowPos);//���̎��̈ʒu

		if (position.z < shotTag->GetPosition().z) {//�ˌ��I�������

			boPosFlag = false;//��x����ǂݍ��݃��Z
			pMemFlag = false;//��x����Z�b�g

			Nowframe = NowframeDef;
			//�����˂�����
			PlungeCompletFlag = true;
			plungeIntoPattern_ = PlungeIntoPattern::Wait;//�҂��Ă���
		}
		break;

	case PlungeIntoPattern::Reverse://�˂����ݏI���������߂�

		ReversePos = { 0,100,shotTag->GetPosition().z + 1000 };

		Nowframe++;

		if (BeforeReversePosMemFlag == false) {
			BeforeReversePosMem = position;
			BeforeReversePosMemFlag = true;
		}

		//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
		ReverseSp.x = (ReversePos.x - BeforeReversePosMem.x);
		ReverseSp.y = (ReversePos.y - BeforeReversePosMem.y);
		ReverseSp.z = (ReversePos.z - BeforeReversePosMem.z);

		//XMVECTOR�ɕϊ�����xmvecMoveSp�ɂ����
		XMVECTOR xmvecRevMoveSp = XMLoadFloat3(&ReverseSp);
		//normalize
		xmvecRevMoveSp = XMVector3Normalize(xmvecRevMoveSp);
		// �傫����C�Ӓl��(���x)
		xmvecRevMoveSp = XMVectorScale(xmvecRevMoveSp, 10.f);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&ReverseSp, xmvecRevMoveSp);

		//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
		ReverseNowPos.x = BeforeReversePosMem.x + ReverseSp.x * Nowframe;
		ReverseNowPos.y = BeforeReversePosMem.y + ReverseSp.y * Nowframe;
		ReverseNowPos.z = BeforeReversePosMem.z + ReverseSp.z * Nowframe;

		obj->SetPosition(ReverseNowPos);//���̎��̈ʒu

		//z���W���w����W�ɂȂ�����
		if (position.z >= ReversePos.z) {
			PlungeCount = PlungeCountDef;
			BeforeReversePosMemFlag = false;
			plungeIntoPattern_ = PlungeIntoPattern::Leave;
			actionPattern_ = ActionPattern::AfterPlungeInto;//�˂����݈�A�I�������̍s����
		}

		break;

	case PlungeIntoPattern::Wait://�ҋ@���Ă���s��
		//---------------------�\���͂����� �V�F�C�N
		//�ҋ@���ԃf�N�������g
		PlungeIntoWaitCount--;

		//�ˌ���Ȃ�˂����񂾏ꏊ�ł܂�
		if (PlungeCompletFlag == true) {
			obj->SetPosition(pPosMem);
		}
		else {
			XMFLOAT3 rot = obj->GetRotation();
			rot.z++;
			obj->SetRotation(rot);
		}

		if (PlungeIntoWaitCount == 0) {
			if (PlungeCompletFlag == false) {//�˂����݊����O�Ȃ�
				//���̎��̃^�[�Q�b�g���W
				//��x����@�˂����݂Ɏg�����W
				if (pMemFlag == false) {
					pPosMem = shotTag->GetPosition();
					pMemFlag = true;
				}

				if (boPosFlag == false)
				{
					//�ŏ��̈ʒu
					boPosMom = obj->GetPosition();
					boPosFlag = true;
				}
				PlungeIntoWaitCount = PlungeIntoWaitCountDef;
				plungeIntoPattern_ = PlungeIntoPattern::PlungeInto;
			}
			else {//�˂����݌�
				PlungeCompletFlag = false;//���Z�b�g
				PlungeIntoWaitCount = PlungeIntoWaitCountDef;
				ShakePosMemFlag = false;//�V�F�C�N�I���
				Nowframe = NowframeDef;//�߂��Ȃ��Ǝ��̍s���ɃJ�N�c�L���o��
				plungeIntoPattern_ = PlungeIntoPattern::Reverse;//���̏ꏊ�֌�����
			}
		}
		break;
	}

}

void Boss::AfterPlungeInto()
{
	switch (afterPlungePattern_)
	{
	case AfterPlungePattern::Wait://�ҋ@���Ă���s��

		WaitTime--;//�҂�����
		if (WaitTime == 0) {//�w�莞�ԑ҂�����

			LoopCount++;//����������������񂾂�
			WaitTime = WaitTimeDef;
			Nowframe = NowframeDef;
			afterPlungePattern_ = AfterPlungePattern::Attack;//�U����
		}

		AtkCount--;
		//������������
		if (AtkCount == 0) {
			//�ˌ����A�������̂ݔ���
			if (alive) { StraightAttack(); }
			//�ĂуJ�E���g�ł���悤�ɏ�����
			AtkCount = AtkInterval;
		}

		break;

	case AfterPlungePattern::Attack:

		Nowframe++;

		if (pPosMemFlag == false) {
			pPosMem = shotTag->GetPosition();//���@�����ꏊ
			boPosMem = obj->GetPosition();//�{�X�����ꏊ
			pPosMemFlag = true;
		}
		//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
		AtkMoveSp.x = (pPosMem.x - boPosMem.x) / NecesAtkMoveTime;
		AtkMoveSp.y = (pPosMem.y - boPosMem.y) / NecesAtkMoveTime;
		AtkMoveSp.z = (pPosMem.z + 800 - boPosMem.z) / NecesAtkMoveTime;

		//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
		boNowPos.x = boPosMem.x + AtkMoveSp.x * Nowframe;
		boNowPos.y = boPosMem.y + AtkMoveSp.y * Nowframe;
		boNowPos.z = boPosMem.z + AtkMoveSp.z * Nowframe;

		obj->SetPosition(boNowPos);//���̎��̈ʒu

		AfterPlungePatAtkCount--;
		//������������
		if (AfterPlungePatAtkCount == 0) {
			//�ˌ����A�������̂ݔ���
			if (alive) { Attack(); }
			//�ĂуJ�E���g�ł���悤�ɏ�����
			AfterPlungePatAtkCount = AfterPlungePatAtkInterval;
		}

		if (Nowframe == NecesAtkMoveTime) {
			Nowframe = NowframeDef;
			pPosMemFlag = false;
			afterPlungePattern_ = AfterPlungePattern::Wait;
		}

		break;
	}

	//���s������萔�J��Ԃ�����s���ς���
	if (LoopCount == LoopCountMax) {
		LoopCount = LoopCountDef;
		afterPlungePattern_ = AfterPlungePattern::Wait;//�����̍s���߂�
		actionPattern_ = ActionPattern::CircularMotionMove;//���̍s��
	}
}

void Boss::Shake() {

	CharParameters* charParameters = CharParameters::GetInstance();
	//pos�h�炷
	XMFLOAT3 pos = obj->GetPosition();

	randShakeNow = 7 + 1;//a~b

	if (ShakePosMemFlag == false) {
		posMem = pos;
		ShakePosMemFlag = true;
	}
	if (ShakePosMemFlag == true) {
		pos.x = posMem.x + rand() % randShakeNow - 4.f;//a~b�܂ł�randShakeNow�̍ő�l���甼���������ĕ��̐����܂ނ悤��
		pos.y = posMem.y + rand() % randShakeNow - 4.f;
	}
	obj->SetPosition(pos);

}

//-------�U���n
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
	std::unique_ptr<BossAimBul> madeAimBullet = std::make_unique<BossAimBul>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeAimBullet->Initialize();
	madeAimBullet->SetModel(AimBulModel);
	madeAimBullet->SetPosition(position);

	//�e�o�^
	aimBullets_.push_back(std::move(madeAimBullet));
}
void Boss::PAimBul()
{
	//�e�����
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {
		//���̎��̃^�[�Q�b�g���W
	//��x����
		if (aimBullet->ShotTagMomOnlyFlag == true) {
			aimBullet->ShotTagMoment = shotTag->GetPosition();
			aimBullet->ShotTagMomOnlyFlag = false;
		}

		aimBullet->Nowframe++;
		if (aimBullet->GetPosOnlyFlag == true)
		{
			//�ŏ��̈ʒu
			aimBullet->boPosMoment = obj->GetPosition();
			aimBullet->GetPosOnlyFlag = false;
		}
		//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
		// //��Γ�����
		////MoveSp.x = (shotTag->GetPosition().x - sePosMoment.x);
		////MoveSp.y = (shotTag->GetPosition().y - sePosMoment.y);
		////MoveSp.z = (shotTag->GetPosition().z - sePosMoment.z);
		aimBullet->MoveSp.x = (aimBullet->ShotTagMoment.x - aimBullet->boPosMoment.x);
		aimBullet->MoveSp.y = (aimBullet->ShotTagMoment.y - aimBullet->boPosMoment.y);
		aimBullet->MoveSp.z = (aimBullet->ShotTagMoment.z - aimBullet->boPosMoment.z);

		//XMVECTOR�ɕϊ�����xmvecMoveSp�ɂ����
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&aimBullet->MoveSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// �傫����C�Ӓl��(���x)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 7.f);
		// FLOAT3�ɕϊ�
		XMStoreFloat3(&aimBullet->MoveSp, xmvecMoveSp);

		//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
		aimBullet->NowPos.x = aimBullet->boPosMoment.x + aimBullet->MoveSp.x * aimBullet->Nowframe;
		aimBullet->NowPos.y = aimBullet->boPosMoment.y + aimBullet->MoveSp.y * aimBullet->Nowframe;
		aimBullet->NowPos.z = aimBullet->boPosMoment.z + aimBullet->MoveSp.z * aimBullet->Nowframe;

		aimBullet->SetPosition(aimBullet->NowPos);//���̎��̈ʒu
	}
}
void Boss::DiffusionAttack()
{
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//�e����
	XMFLOAT3 position = obj->GetPosition();
	//�e����
	std::unique_ptr<BossBullet> madeBullet_center = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_L = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_R = std::make_unique<BossBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet_center->Initialize();
	madeBullet_L->Initialize();
	madeBullet_R->Initialize();

	madeBullet_center->SetModel(BulModel);
	madeBullet_L->SetModel(BulModel);
	madeBullet_R->SetModel(BulModel);

	madeBullet_center->SetPosition(position);
	madeBullet_L->SetPosition(position);
	madeBullet_R->SetPosition(position);

	// velocity���Z�o
	const float MoveZ = 1.7f;//�ړ���
	const float MoveX = 2.f;
	DirectX::XMVECTOR vecvelocity_center = DirectX::XMVectorSet(0, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_L = DirectX::XMVectorSet(-MoveX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_R = DirectX::XMVectorSet(MoveX, 0, MoveZ, 0);
	XMFLOAT3 xmfloat3velocity_center;
	XMFLOAT3 xmfloat3velocity_L;
	XMFLOAT3 xmfloat3velocity_R;
	XMStoreFloat3(&xmfloat3velocity_center, XMVector3Transform(vecvelocity_center, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_L, XMVector3Transform(vecvelocity_L, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_R, XMVector3Transform(vecvelocity_R, obj->GetMatRot()));

	madeBullet_center->SetVelocity(xmfloat3velocity_center);
	madeBullet_L->SetVelocity(xmfloat3velocity_L);
	madeBullet_R->SetVelocity(xmfloat3velocity_R);

	//�e�o�^
	bullets_.push_back(std::move(madeBullet_center));
	bullets_.push_back(std::move(madeBullet_L));
	bullets_.push_back(std::move(madeBullet_R));
}
void Boss::DiffusionAttackEavenNumber()
{
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//�e����
	XMFLOAT3 position = obj->GetPosition();
	//�e����
	std::unique_ptr<BossBullet> madeBullet_L1 = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_L2 = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_R1 = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_R2 = std::make_unique<BossBullet>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet_L1->Initialize();
	madeBullet_L2->Initialize();
	madeBullet_R1->Initialize();
	madeBullet_R2->Initialize();

	madeBullet_L1->SetModel(BulModel);
	madeBullet_L2->SetModel(BulModel);
	madeBullet_R1->SetModel(BulModel);
	madeBullet_R2->SetModel(BulModel);

	madeBullet_L1->SetPosition(position);
	madeBullet_L2->SetPosition(position);
	madeBullet_R1->SetPosition(position);
	madeBullet_R2->SetPosition(position);

	// velocity���Z�o
	const float MoveZ = 1.7f;//�ړ���
	const float MoveEdgeX = 3.f;//�[�e�ړ���
	const float MoveX = 0.5f;//�ړ���
	DirectX::XMVECTOR vecvelocity_L1 = DirectX::XMVectorSet(-MoveEdgeX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_L2 = DirectX::XMVectorSet(-MoveX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_R1 = DirectX::XMVectorSet(MoveX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_R2 = DirectX::XMVectorSet(MoveEdgeX, 0, MoveZ, 0);
	XMFLOAT3 xmfloat3velocity_L1;
	XMFLOAT3 xmfloat3velocity_L2;
	XMFLOAT3 xmfloat3velocity_R1;
	XMFLOAT3 xmfloat3velocity_R2;
	XMStoreFloat3(&xmfloat3velocity_L1, XMVector3Transform(vecvelocity_L1, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_L2, XMVector3Transform(vecvelocity_L2, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_R1, XMVector3Transform(vecvelocity_R1, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_R2, XMVector3Transform(vecvelocity_R2, obj->GetMatRot()));

	madeBullet_L1->SetVelocity(xmfloat3velocity_L1);
	madeBullet_L2->SetVelocity(xmfloat3velocity_L2);
	madeBullet_R1->SetVelocity(xmfloat3velocity_R1);
	madeBullet_R2->SetVelocity(xmfloat3velocity_R2);

	//�e�o�^
	bullets_.push_back(std::move(madeBullet_L1));
	bullets_.push_back(std::move(madeBullet_L2));
	bullets_.push_back(std::move(madeBullet_R1));
	bullets_.push_back(std::move(madeBullet_R2));
}
void Boss::StraightAttack()
{
	// �����Đ� �炵�����Ƃ�
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//�e����
	XMFLOAT3 position = obj->GetPosition();
	//�e����
	std::unique_ptr<BossStraightBul> madeBullet = std::make_unique<BossStraightBul>();
	//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
	madeBullet->Initialize();
	madeBullet->SetModel(StraightBulModel);
	madeBullet->SetPosition(position);
	madeBullet->SetScale({ 30.f, 30.f, 30.f });

	//�e�o�^
	straightBullets_.push_back(std::move(madeBullet));
}

//------�U���n��
void Boss::Death() {

	Nowframe++;
	ParticleFrame++;
	PartTimeInterval = ParticleFrame / 40;

	if (GetPosDeathOnlyFlag == true)
	{
		//�ŏ��̈ʒu
		boPosDeath = obj->GetPosition();
		GetPosDeathOnlyFlag = false;
	}

	//XMFLOAT3 boRot = obj->GetRotation();
	//�ړ����x���i�w����W-�ŏ��ʒu�j/�����鎞��
	MoveSp.x = (boPosDeath.x - boPosDeath.x) / NecesFrame;//�w����WX�͎��񂾏ꏊ����^���ɗ�����悤�Ɏ���X���W
	MoveSp.y = (TargetPos.y - boPosDeath.y) / NecesFrame;
	MoveSp.z = (boPosDeath.z - boPosDeath.z) / NecesFrame;//�V
	//���̎��̈ʒu���ŏ��ʒu�{�ړ����x���o�ߎ���
	NowPos.x = boPosDeath.x + MoveSp.x * Nowframe;
	NowPos.y = boPosDeath.y + MoveSp.y * Nowframe;
	NowPos.z = boPosDeath.z + MoveSp.z * Nowframe;

	//boRot.y += +1.0;
	//boPos.y += -0.5;
	//ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
	obj->SetPosition(NowPos);//���̎��̈ʒu

	//��莞�Ԃ��ƂɃp�[�e�B�N��
	if (PartTimeInterval == 1) {
		// �����Đ� �炵�����Ƃ�
		GameSound::GetInstance()->PlayWave("destruction1.wav", 0.2f);
		ParticleManager::GetInstance()->CreateParticle(NowPos, 100, 80, 10);
		PartTimeInterval = 0;
		ParticleFrame = 0;
	}
	//obj->SetRotation(boRot);
}

void Boss::Initialize()
{

	//���
	obj.reset(Object3d::Create());
	//-----���C�Ӂ�-----//
	//�傫��
	obj->SetScale({ 60.0f, 60.0f, 60.0f });
	//�ꏊ
	obj->SetPosition({ 0,0,3300 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");
	GameSound::GetInstance()->LoadWave("destruction1.wav");

	//�߂Â��p�^�[��������
	ApproachInit();
}

void Boss::Update()
{
	Input* input = Input::GetInstance();
	CharParameters* charParameters = CharParameters::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;
	frame += 1.f;

	//���Ńt���O�������炻�̒e�͎����Ĕq����
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return !bullet->GetAlive();
		});
	aimBullets_.remove_if([](std::unique_ptr<BossAimBul>& aimBullet) {
		return !aimBullet->GetAlive();
		});
	straightBullets_.remove_if([](std::unique_ptr<BossStraightBul>& straightBullet) {
		return !straightBullet->GetAlive();
		});

	//�����o�֐��|�C���^�Ή������{�X�̓���������
	if (actionPattern_ == ActionPattern::BossAppear) { pFunc = &Boss::BossAppear; }
	if (actionPattern_ == ActionPattern::Approach) { pFunc = &Boss::Approach; }
	if (actionPattern_ == ActionPattern::Leave) { pFunc = &Boss::Leave; }
	if (actionPattern_ == ActionPattern::Vertical) { pFunc = &Boss::Vertical; }
	if (actionPattern_ == ActionPattern::HpHalfPatStart) { pFunc = &Boss::HpHalfPatStart; }
	if (actionPattern_ == ActionPattern::Death) { pFunc = &Boss::Death; }
	if (actionPattern_ == ActionPattern::CircularMotionMove) { pFunc = &Boss::CircularMotionMove; }
	if (actionPattern_ == ActionPattern::LeaveFirstPos) { pFunc = &Boss::LeaveFirstPos; }
	if (actionPattern_ == ActionPattern::PlungeInto) { pFunc = &Boss::PlungeInto; }
	if (actionPattern_ == ActionPattern::AfterPlungeInto) { pFunc = &Boss::AfterPlungeInto; }

	if (isDeath == true) {
		actionPattern_ = ActionPattern::Death;
		if (IsFirst_Death == false) {
			Nowframe = NowframeDef;
			IsFirst_Death = true;
		}
	}
	if (isHpHalfPattern == false) {
		if (charParameters->GetNowBoHp() <= charParameters->GetboMaxHp() / 2) {
			actionPattern_ = ActionPattern::HpHalfPatStart;
		}
	}

	if (plungeIntoPattern_ == PlungeIntoPattern::Wait) {
		Shake();
	}

	//�����o�֐��|�C���^�Ăяo��
	(this->*pFunc)();

	//�e�X�V
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {//�_���e
		aimBullet->Update();
	}
	for (std::unique_ptr<BossStraightBul>& straightBullet : straightBullets_) {//�_���e
		straightBullet->Update();
	}

	//�_���e
	PAimBul();

	obj->Update();
}

void Boss::Draw()
{
	//�e
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {//�_���e
		aimBullet->Draw();
	}
	for (std::unique_ptr<BossStraightBul>& straightBullet : straightBullets_) {//�����e
		straightBullet->Draw();
	}

	if (alive) {
		obj->Draw();
	}
}
