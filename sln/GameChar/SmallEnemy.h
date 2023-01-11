#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"
#include "SmallEnemyBullet.h"

#include <memory>

class SmallEnemy:public BaseObject
{
	//�J���p�^�[��
	enum class RetirePat {
		Right,
		Left,
		def,
	};

private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Attack();

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	std::unique_ptr<Camera> camera; //�J����

	//������܂�
	static const int32_t BulVanishTime = 60*7;//����
	//���ł̐鍐
	int32_t vanishTimer_ = BulVanishTime;

	//���E�ɎJ����
	static const int32_t RetireFrameDef = 120;
	int32_t RetireFrame = RetireFrameDef;

	std::list <std::unique_ptr<SmallEnemyBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//-----------------���������[�������[��------------------//
	//�e���X�g���擾
	const std::list<std::unique_ptr<SmallEnemyBullet>>& GetBullets() { return bullets_; }

	inline void SetSEBulModel(Model* model) { seBulModel = model; }

	//�ʏ�e�З�
	void SetBulPow(float seBulPower) { this->seBulPower = seBulPower; }
	const float& GetBulPow() { return seBulPower; }

	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }
	//-----------------���������[�������[��------------------//

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 90;

	RetirePat retirePat_ = RetirePat::def;

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

	//SEneX�̗��������ϐ�
	float SEneRandX = 0;

	//�U���p�J�E���g
	float AtkCount = 0;
	Model* seBulModel = nullptr;

	//�ʏ�e�З�
	const float seBulPowerMax = 100;
	float seBulPower = seBulPowerMax;

	BaseObject* shotTag;//�e���^�[�Q�b�g

	//�����܂ŗ�����~�܂��ĎJ����
	const int PosZMax = 270;
	//�J���J�n
	bool isRetire = false;
	//�������Ă���Ԃ���
	bool isSeApproach = true;

};