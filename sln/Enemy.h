#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "EnemyBullet.h"
#include "BaseObject.h"

#include <memory>

class Enemy:public BaseObject
{

	//�s���p�^�[�� enumclass
	enum class ActionPattern {
		Approach,//�߂Â��Ă���
		Leave,//�����
	};

public:

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�U��
	void Attack();

	//�߂Â�����������
	void ApproachInit();

	std::unique_ptr<Camera> camera; //�J����
	
	float time;

	//�߂Â�����
	const float ApproachSp = 1;
	//�߂Â��鋗��
	const float ApproachLim = -50;
	//������鋗��
	const float LeaveLim = 90;

	//�����p�^�[�� enumclass�́@�@�@�@�^���@�@:: �@�񋓎q
	ActionPattern actionPattern_ = ActionPattern::Approach;

	std::list <std::unique_ptr<EnemyBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//�t���[�����Ƃɔ���
	static const int AtkInterval = 30;

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

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

	//�t���[��
	float frame = 0;
	//�U���p�J�E���g
	float AtkCount = 0;
};