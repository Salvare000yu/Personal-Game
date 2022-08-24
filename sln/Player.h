#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"

#include <memory>
#include <list>//STL

class Player
{
public:
	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�U��
	void Attack();

	static Player* GetInstance();

	std::unique_ptr<Camera> camera; //�J����

	float time;

	float PlayerposX = 0;
	float PlayerposY = 0;
	float PlayerposZ = 0;

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//���f��
	std::unique_ptr < Model> mod_player = nullptr;
	//�I�u�W�F3d
	std::unique_ptr < Object3d> obj_player = nullptr;

	//�����������Ăяo�����߂̃R�[���o�b�N�֐�
	void OnCollision();
	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

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
};

