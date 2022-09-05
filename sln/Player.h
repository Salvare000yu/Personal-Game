#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "BaseObject.h"

#include <memory>
#include <list>//STL

class Player:public BaseObject
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
public:
	//������
	void Initialize() override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�U��
	void Attack();

	std::unique_ptr<Camera> camera; //�J����

	float time;

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//���f��
	std::unique_ptr < Model> mod_player = nullptr;

	//�����������Ăяo�����߂̃R�[���o�b�N�֐�
	void OnCollision();
	//�e���X�g���擾 gamescene�Ɏ��e�݂�����
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//�e���W����Ŏg�����߂�getbulletposmemory
	static XMFLOAT3 Player::GetPlayerPosMemory();
	//�ÓI�����o�ϐ��@�����蔻��Ŏg���ׂ̒e���W
	static XMFLOAT3 PlayerPosMemory;

};

