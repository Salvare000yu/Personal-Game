#pragma once
#include "Object3d.h"
#include "BaseObject.h"

#include <memory>

class PlayerBullet:public BaseObject
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

	float frame = 0;

	XMFLOAT3 velocity{ 0,0,4 };

public:
	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	std::unique_ptr<Camera> camera; //�J����

	float time;

	//�e���W����Ŏg�����߂�getbulletposmemory
	static XMFLOAT3 PlayerBullet::GetPlayerBulPosMemory();
	//�ÓI�����o�ϐ��@�����蔻��Ŏg���ׂ̒e���W
	static XMFLOAT3 PlayerBulPosMemory;

	//������܂�
	static const int32_t BulVanishTime = 60 * 5;//����

	//���ł̐鍐
	int32_t vanishTimer_ = BulVanishTime;

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }

};
