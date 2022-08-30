#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"

#include <memory>

class SmallEnemy
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

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	static SmallEnemy* GetInstance();

	std::unique_ptr<Camera> camera; //�J����

	//������܂�
	static const int32_t BulVanishTime = 60*6;//����

	//���ł̐鍐
	int32_t vanishTimer_ = BulVanishTime;

	bool IsVanish()const { return isVanish_; }

	//�����������Ăяo�����߂̃R�[���o�b�N�֐�
	void OnCollision();

	//�G���G���W����Ŏg�����߂�getbulletposmemory
	//static XMFLOAT3 SmallEnemy::GetSEnePosMemory(void) { return SEnePosMemory; }
	//�ÓI�����o�ϐ��@�����蔻��Ŏg���ׂ̎G���G���W
	//static XMFLOAT3 SEnePosMemory;

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

	std::unique_ptr < Model> mod_smallenemy = nullptr;

	std::unique_ptr < Object3d> obj_smallenemy = nullptr;

	//���Ńt���O
	bool isVanish_ = FALSE;

	//�����_��x�ő�ŏ�
	const int SEneMaxX = 50;
	const int SEneMinX = -50;
	//SEneX�̗��������ϐ�
	float SEneRandX = 0;
};