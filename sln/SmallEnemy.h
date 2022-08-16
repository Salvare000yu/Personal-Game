#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"

#include <memory>

class SmallEnemy
{

public:

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	static SmallEnemy* GetInstance();

	std::unique_ptr<Camera> camera; //�J����

	float time;

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 30;

	//������܂�
	static const int32_t BulVanishTime = 60 * 5;//����

		//���ł̐鍐
	int32_t vanishTimer_ = BulVanishTime;

	bool IsVanish()const { return isVanish_; }

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

	//�t���[��
	float frame = 0;
	//�U���p�J�E���g
	float AtkCount = 0;

	//���Ńt���O
	bool isVanish_ = FALSE;
};