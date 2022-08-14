#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "EnemyBullet.h"

#include <memory>

class Enemy
{

	//�s���p�^�[�� enumclass
	enum class ActionPattern {
		Approach,//�߂Â��Ă���
		Leave,//�����
	};

public:

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�U��
	void Attack();

	//�߂Â�����������
	void ApproachInit();

	static Enemy* GetInstance();

	std::unique_ptr<Camera> camera; //�J����
	
	float time;

	//�߂Â�����
	const float ApproachSp = 1;
	//�߂Â��鋗��
	const float ApproachLim = -50;
	//������鋗��
	const float LeaveLim = 50;

	//�����p�^�[�� enumclass�́@�@�@�@�^���@�@:: �@�񋓎q
	ActionPattern actionPattern_ = ActionPattern::Approach;

	std::list <std::unique_ptr<EnemyBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 30;

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

	//�t���[��
	float frame = 0;
	//�U���p�J�E���g
	float AtkCount = 0;
};