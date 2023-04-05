#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class SmallEnemyBullet :public BaseObject
{
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

	//float frame = 0;

	XMFLOAT3 velocity{ 0,0,0 };

public:
	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	static SmallEnemyBullet* GetInstance();

	std::unique_ptr<Camera> camera; //�J����

	//float time;

	//������܂�
	static const int32_t BulVanishTime = 60 * 5;//����

	//���ł̐鍐
	int32_t vanishTimer_ = BulVanishTime;

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }

	float Nowframe = 0;//���݃t��
	bool GetPosOnlyFlag = true;//��x����̍��W�ǂݍ���
	XMFLOAT3 NowPos;//���̎��̒e�ʒu
	XMFLOAT3 sePosMoment;//���ˎ��̎G���G�ʒu
	XMFLOAT3 MoveSp;//�e�ړ����x

	bool ShotTagMomOnlyFlag = true;//��x����̃^�[�Q�b�g���W�ǂݍ���
	XMFLOAT3 ShotTagMoment{};

private:

	float tmp;
	short angle;
};
