#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"

#include <memory>

class Enemy
{
public:
	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	static Enemy* GetInstance();

	std::unique_ptr<Camera> camera; //�J����
	
	float time;

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

	std::unique_ptr < Model> mod_enemy = nullptr;

	std::unique_ptr < Object3d> obj_enemy = nullptr;

	float frame = 0;
};

