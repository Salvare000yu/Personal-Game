#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"
#include "SmallEnemyBullet.h"

#include <memory>

class SmallEnemy:public BaseObject
{
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
	static const int32_t BulVanishTime = 60*5+20;//����

	//���ł̐鍐
	int32_t vanishTimer_ = BulVanishTime;

	std::list <std::unique_ptr<SmallEnemyBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//�e���X�g���擾
	const std::list<std::unique_ptr<SmallEnemyBullet>>& GetBullets() { return bullets_; }

	inline void SetSEBulModel(Model* model) { seBulModel = model; }

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 60;

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
};