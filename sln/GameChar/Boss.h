#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BossBullet.h"
#include "BaseObject.h"

#include <memory>

class Boss:public BaseObject
{

	//�s���p�^�[�� enumclass
	enum class ActionPattern {
		Approach,//�߂Â��Ă���
		Leave,//�����
		HpHalfPatStart,//Hp�����ȉ��ɂȂ�����^�Q�܂ňړ�
		HpHalf,
		Death,//���S
	};

public:

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�U��
	void Attack();
	void Approach();
	void Leave();
	void HpHalfPatStart();
	void HpHalf();
	//�g�U�U��
	void DiffusionAttack();
	//�g�U�����e
	void DiffusionAttackEavenNumber();

	void Death();

	//�߂Â�����������
	void ApproachInit();

	std::unique_ptr<Camera> camera; //�J����
	
	float time;

	//�߂Â�����
	const float ApproachSpZ = 1;
	const float ApproachSpY = 2;
	//����鑬��
	const float LeaveSpZ = 1;
	const float LeaveSpY = 2;
	//�߂Â��鋗��
	const float ApproachLim = -50;
	//������鋗��
	const float LeaveLim = 90;

	void  (Boss::* pFunc)();

	//�����p�^�[�� enumclass�́@�@�@�@�^���@�@:: �@�񋓎q
	ActionPattern actionPattern_ = ActionPattern::Approach;

	std::list <std::unique_ptr<BossBullet>> bullets_;//�{�X�̒e�@���j�[�N�|�C���^

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 10;
	static const int DiffusionAtkInterval =20;

	//-----------------���������[�������[��------------------//
	//�e���X�g���擾
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }

	inline void SetEBulModel(Model* model) { eBulModel = model; }

	//�{�X�h���
	void SetBossDefense(float BossDefense) { this->BossDefense = BossDefense; }
	const float& GetBossDefense() { return BossDefense; }

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

	bool even_odd_NumFlag = true;//��e

	Model* eBulModel = nullptr;

	//�h���
	const float BossDefenseDef = 10;
	float BossDefense = BossDefenseDef;

	//false�F�܂�����łȂ�
	bool isDeath = false;

	XMFLOAT3 boPosDeath = {};
	bool GetPosFlag = true;//��x����̍��W�ǂݍ���
	const float NecesFrame = 190.0f;//�����鎞��
	float Nowframe = 0;//���݃t��
	XMFLOAT3 MoveSp = {};//�ړ����x
	XMFLOAT3 TargetPos = { 0,-150,0 };//�ڕW���W
	XMFLOAT3 NowPos = {};//���̎��̈ʒu

	float PartTimeInterval;
	float ParticleFrame = 39;//�p�[�e�B�N���o���t��

	//-------��HPHALF��------//
	bool isHpHalfPattern = false;//hp�����ȉ��s�����ĂȂ�

	XMFLOAT3 HpHalfMomentPos = {};
	//�܂����̈ʒu�ɍs��
	XMFLOAT3 TargetHpHalfPos = { 0, 40, 200 };
	const float NecesHpHalfFrame = 180.0f;//HP���������̃t���[�����ړ��܂Ŏ��Ԃ�����
	//-------��HPHALF��------//

	BaseObject* shotTag;//�e���^�[�Q�b�g
};