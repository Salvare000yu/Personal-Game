#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BossBullet.h"
#include "BossAimBul.h"
#include "BossStraightBul.h"
#include "BaseObject.h"

#include <memory>

class Boss:public BaseObject
{

	//�s���p�^�[�� enumclass
	enum class ActionPattern {
		BossAppear,
		Approach,//�߂Â��Ă���
		Leave,//�����
		HpHalfPatStart,//Hp�����ȉ��ɂȂ�����^�Q�܂ňړ�
		CircularMotionMove,//���邮��
		LeaveFirstPos,//���̏ꏊ�ֈ�U����
		PlungeInto,//�˂�����
		AfterPlungeInto,//�˂����݌�
		Death,//���S
	};

	enum class PlungeIntoPattern {
		Leave,//�O�����ŗ����
		PlungeInto,//�˂�����
		Reverse,//�߂��Ă���
		Wait,//���ԋ󂯂Ă���s��
	};

	enum class AfterPlungePattern {
		Wait,//���ԋ󂯂Ă���s��
		Attack,//�U��
	};

public:

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�V�F�C�N
	void Shake();

	//�U��
	void Attack();

	void PAimBul();
	void BossAppear();
	void Approach();
	void Leave();
	void HpHalfPatStart();
	void CircularMotionMove();
	void LeaveFirstPos();
	void PlungeInto();
	void AfterPlungeInto();
	//�g�U�U��
	void DiffusionAttack();
	//�g�U�����e
	void DiffusionAttackEavenNumber();
	//�����U��
	void StraightAttack();

	void Death();

	//�߂Â�����������
	void ApproachInit();

	std::unique_ptr<Camera> camera; //�J����
	
	float time;

	//�߂Â�����
	const float ApproachSpZ = 1.f;
	const float ApproachSpY = 2.f;
	//����鑬��
	const float LeaveSpZ = 1.f;
	const float LeaveSpY = 2.f;
	//�߂Â��鋗��
	float ApproachLim;
	//������鋗��
	float LeaveLim = 90;

	void  (Boss::* pFunc)();

	//�����p�^�[�� enumclass�́@�@�@�@�^���@�@:: �@�񋓎q
	ActionPattern actionPattern_ = ActionPattern::BossAppear;

	std::list <std::unique_ptr<BossBullet>> bullets_;//�{�X�̒e�@���j�[�N�|�C���^
	std::list <std::unique_ptr<BossAimBul>> aimBullets_;//�{�X�̑_���e
	std::list <std::unique_ptr<BossStraightBul>> straightBullets_;//�{�X�̒����e

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 10;
	static const int AtkInterval_LeaveFirst = 20;
	static const int DiffusionAtkInterval =20;

	//��x����Ă���˂����ލs���p�^�[���@�ŏ������
	PlungeIntoPattern plungeIntoPattern_ = PlungeIntoPattern::Leave;

	//�˂����݌�̍s���p�^�[�� �ŏ��҂�
	AfterPlungePattern afterPlungePattern_ = AfterPlungePattern::Wait;

	//-----------------���������[�������[��------------------//
	//�e���X�g���擾
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }
	const std::list<std::unique_ptr<BossAimBul>>& GetAimBullets() { return aimBullets_; }
	const std::list<std::unique_ptr<BossStraightBul>>& GetStraightBullets() { return straightBullets_; }

	inline void SetBulModel(Model* model) { BulModel = model; }
	inline void SetAimBulModel(Model* model) { AimBulModel = model; }
	inline void SetStraightBulModel(Model* model) { StraightBulModel = model; }

	//�ʏ�e�З�
	const float& GetBulPow() { return BulPow; }
	//�_���e�З�
	const float& GetAimBulPow() { return AimBulPow; }
	//�{�X�{�̂ɂԂ��������̈З�
	const float& GetBodyPow() { return BodyPow; }

	//�{�X����
	void SetisDeath(float isDeath) { this->isDeath = isDeath; }
	const float& GetisDeath() { return isDeath; }

	//�_���e�@�ł���
	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }
	//-----------------���������[�������[��------------------//

private:

	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//�t���[��
	float frame = 0;
	//�U���p�J�E���g
	float AtkCount = 0;
	float DiffusionAtkCount = 0;
	float Circular_AtkCount = 0;	//���邮�鎞�U���p�J�E���g
	float AfterPlungePatAtkCount = 10;//�˂����݌�s���̍U���J�E���g

	bool even_odd_NumFlag = true;//��e

	Model* BulModel = nullptr;
	Model* AimBulModel = nullptr;
	Model* StraightBulModel = nullptr;

	//�G�ʏ�e�З�
	const float BulPow = 200;
	const float AimBulPow = 150;

	//�{�X�{�̂̈З�
	const int BodyPow = 100;

	//false�F�܂�����łȂ�
	bool isDeath = false;

	//���񂾂Ƃ���x
	bool IsFirst_Death = false;

	XMFLOAT3 boPosDeath = {};
	bool GetPosFlag = true;//��x����̍��W�ǂݍ���
	bool GetPosDeathFlag = true;//��x����̍��W�ǂݍ���
	const float NecesFrame = 190.0f;//�����鎞��
	float Nowframe = 0;//���݃t��
	XMFLOAT3 MoveSp = {};//�ړ����x
	XMFLOAT3 TargetPos = { 0,-150,0 };//�ڕW���W
	XMFLOAT3 NowPos = {};//���̎��̈ʒu

	float PartTimeInterval;
	float ParticleFrame = 39;//�p�[�e�B�N���o���t��

	//------HP�����ȉ��~�^����
	const float HpHalf_AngleDef = 20;
	float HpHalf_Angle = HpHalf_AngleDef;
	const float HpHalf_radDef = 100;
	float HpHalf_rad = HpHalf_radDef;

	const float HpHalf_LengthDef = 10;//�~�^���̔��a�̒���
	float HpHalf_Length = HpHalf_LengthDef;
	//�ړ��l
	const float addXDef=0;
	float addX= addXDef;
	const float addYDef = 0;
	float addY = addYDef;
	//------HP�����ȉ��~�^����

	static const int Circular_AtkIntervalDef = 45;
	int Circular_AtkInterval = Circular_AtkIntervalDef;

	//�˂����݌�s����AimBul�C���^�[�o��
	static const int AfterPlungePatAtkInterval = 15;

	//-------��HPHALF��------//
	bool isHpHalfPattern = false;//hp�����ȉ��s�����ĂȂ�

	XMFLOAT3 HpHalfMomentPos = {};
	//�܂����̈ʒu�ɍs��
	XMFLOAT3 TargetHpHalfPos;
	const float NecesHpHalfFrame = 100.0f;//HP���������̃t���[�����ړ��܂Ŏ��Ԃ�����

	const float NecesLeaveFirstFrame = 180.f;
	//-------��HPHALF��------//

	BaseObject* shotTag;//�e���^�[�Q�b�g

	//�o�ꂩ��s�����J�n�Ɉڂ������̍��W
	XMFLOAT3 ActionStartPos;

	//-------PlungeInto
	//2��Leave��������˂����ލs���@�@�f�t�H2
	const int PlungeCountDef = 1;
	int PlungeCount = PlungeCountDef;
	//�˂����ݍs���ֈڍs����O�ɍŌ�ɂ����ꏊ���L������
	XMFLOAT3 WasPosMem;
	//����鑬�x
	const int LeaveVel = 20;
	//Leave�̎��ǂ̒��x�����邩
	const int LeavePos = 5000;
	//�߂鑬�x
	XMFLOAT3 ReverseSp;
	//�߂�O�̍��W�擾�t���O�@false:���ĂȂ�
	bool BeforeReversePosMemFlag = false;
	//�߂�O���W�����
	XMFLOAT3 BeforeReversePosMem;
	//���̏ꏊ
	XMFLOAT3 ReverseNowPos;
	//�ǂ��ɖ߂邩�̏ꏊ
	XMFLOAT3 ReversePos;
	//�˂����ݑ҂��ҋ@���� �łӂ�180
	const int PlungeIntoWaitCountDef = 180;
	int PlungeIntoWaitCount = PlungeIntoWaitCountDef;
	//�ˌ����x
	const int PlungeVel = 50;

	//----plunge��after�������Ȃ��̂�after�ł��g��
	//���@�����ꏊ�擾�t���O false:���擾
	bool pMemFlag=false;
	//���@�����ꏊ 
	XMFLOAT3 pPosMem{};

	//�˂����ݏI�������  false:�܂��˂�����łȂ�
	bool PlungeCompletFlag = false;
	//�˂�����
	//float PlungerFame = 0;
	//�{�X�ŏ��̏ꏊ
	bool boPosFlag = false;
	//�{�X�����ꏊ
	XMFLOAT3 boPosMom{};
	//�˂����ݑ��x
	XMFLOAT3 PlungeSp{};
	//���̎��̃{�X�̈ʒu
	XMFLOAT3 PlungeNowPos;
	//-------PlungeInto

	//-------AfterPlungeInto
	//
	//�҂�����
	const int WaitTimeDef = 100;
	int WaitTime = WaitTimeDef;
	//�U�����邽�߂̎��@�����ꏊ�擾�t���O false:�擾�O
	bool pPosMemFlag = false;
	//�U�����Ɏ��@�̂����ꏊ�ցiXY�j�ړ����鑬�x
	XMFLOAT3 AtkMoveSp;
	//�{�X�����ꏊ
	XMFLOAT3 boPosMem;
	//�{�X�̌��ݒn
	XMFLOAT3 boNowPos;
	//�U���Ɉړ��ɂ������Ύ���
	const int NecesAtkMoveTime = 160;
	
	// 
	//-------AfterPlungeInto

	//---�V�F�C�N
	int randShakeDef = 0;
	int randShakeNow = randShakeDef;
	//�����ꏊ�ۑ� false:�ۑ��O
	bool ShakePosMemFlag = false;
	//�h�炵�I������ false:�܂�
	bool isShakeFinished = false;
	//�����ꏊ
	XMFLOAT3 posMem={};

};