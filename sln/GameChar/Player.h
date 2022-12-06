#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "BaseObject.h"
#include "PlayerFireLine.h"

#include <memory>
#include <list>//STL

class Player :public BaseObject
{

	DirectX::XMVECTOR lookVec{};

	DirectX::XMVECTOR position{};
private:
	////-----------------model
	//std::unique_ptr < Model> mod_classplayer = nullptr;//���@

	////-----------------obj
	//std::unique_ptr < Object3d> obj_classplayer = nullptr;//���@

	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	Model* pBulModel = nullptr;
	Model* pFiringLine = nullptr;
	std::unique_ptr < PlayerFireLine> firingline_ = nullptr;

	//�U���̃C���^�[�o���̂��߂̃t���O ���ˑO
	bool AttackIntervalFlag = false;

public:
	inline void SetPBulModel(Model* model) { pBulModel=model; }
	inline void SetPFiringLine(Model* model) { pFiringLine = model; }

	/*static Player* GetInstance();*/

	//������
	void Initialize() override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�U��
	void Attack();

	void Move();

	void Shake();

	void PlayerDeath();

	void FiringLine();

	std::unique_ptr<Camera> camera; //�J����

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//�e���X�g���擾 gamescene�Ɏ��e�݂�����
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	Player()
		: lookVec(DirectX::XMVectorSet(0, 0, 1, 0)),
		position(DirectX::XMVectorSet(0, 0, 0, 1)) {
	}
	//�����擾
	inline const DirectX::XMVECTOR& GetLookVec() { return lookVec; }
	inline DirectX::XMVECTOR GetPosVec() { return position; }

	//�U���C���^�[�o��
	static const int32_t AtkInterval = 10;
	//�U���C���^�[�o���^�C�}�[
	int32_t AtkInterval_ = AtkInterval;

	//�h��鎞��
	static const int32_t pShakeTime = 60 / 2;
	//�h�ꂽ���܁[
	int32_t pShakeTimer_ = pShakeTime;

	//-------------------���������[�������[��-------------------//
	//�e�З�
	void SetpBulPow(float pBulPower) { this->pBulPower = pBulPower; }
	const float& GetpBulPow() { return pBulPower; }
	//�v���C�����ǂ���
	void SetReadyNow(bool ReadyNowFlag) { this->ReadyNowFlag = ReadyNowFlag; }
	const bool& GetReadyNow() { return ReadyNowFlag; }
	//���@�̗͂�0��菭�Ȃ���������
	void SetPHpLessThan0(bool isPHpLessThan0) { this->isPHpLessThan0 = isPHpLessThan0; }
	const bool& GetPHpLessThan0() { return isPHpLessThan0; }
	//���@���ۊm�F
	void SetpDeath(bool PlayerDeathFlag) { this->PlayerDeathFlag = PlayerDeathFlag; }
	const bool& GetpDeath() { return PlayerDeathFlag; }
	//-------------------���������[�������[��-------------------//

private:
	//���@�ʏ�e�З�
	const float pBulPowerMax = 200;
	float pBulPower = pBulPowerMax;

	//play��Ready�����ǂ���
	bool ReadyNowFlag = true;//true�F����Ă�
	//���@�̗͂�0��菭�Ȃ��Ƃ� false:�@�O��葽��
	bool isPHpLessThan0 = false;

	const float pDeathRot = 0.4f;

	//-----���@���S���o
	XMFLOAT3 pPosDeath = {};
	float Nowframe = 0;//���݃t��
	const float NecesFrame = 180.0f;//�����鎞��
	XMFLOAT3 MoveSp = {};//�ړ����x
	XMFLOAT3 TargetPos = { 0,-150,0 };//�ڕW���W
	XMFLOAT3 NowPos = {};//���̎��̈ʒu

	float PartTimeInterval;
	float ParticleFrame = 39;//�p�[�e�B�N���o���t��

	bool GetPosFlag = true;//��x����̍��W�ǂݍ���

	bool PlayerDeathFlag = false;//���@���S�@false:������

	//�h��
	int randShakeDef = 0;
	int randShakeNow = randShakeDef;

	//���@���S���o���@false:�������ĂȂ�
	bool ExplosionFlag = false;
};

