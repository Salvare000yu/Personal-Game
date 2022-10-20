#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "BaseObject.h"
#include "PlayerFireLine.h"

#include <memory>
#include <list>//STL

class Player :public BaseObject
{

	DirectX::XMVECTOR lookVec{};

	DirectX::XMVECTOR position{};
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

	Model* pBulModel = nullptr;
	Model* pFiringLine = nullptr;
	std::unique_ptr < PlayerFireLine> firingline_ = nullptr;

	//�U���̃C���^�[�o���̂��߂̃t���O ���ˑO
	bool AttackIntervalFlag = false;

public:
	inline void SetPBulModel(Model* model) { pBulModel=model; }
	inline void SetPFiringLine(Model* model) { pFiringLine = model; }

	//������
	void Initialize() override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;

	//�U��
	void Attack();

	void FiringLine();

	std::unique_ptr<Camera> camera; //�J����

	//float time;

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//�v���C���[�̒e�@���j�[�N�|�C���^

	//�e���X�g���擾 gamescene�Ɏ��e�݂�����
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	Player()
		: lookVec(DirectX::XMVectorSet(0, 0, 1, 0)),
		position(DirectX::XMVectorSet(0, 0, 0, 1)) {
	}
	//�����擾
	inline const DirectX::XMVECTOR& GetLookVec() { return lookVec; }
	inline DirectX::XMVECTOR GetPosVec() { return position; }

	//�U���C���^�[�o��
	static const int32_t AtkInterval = 10;
	//�U���C���^�[�o���^�C�}�[
	int32_t AtkInterval_ = AtkInterval;
};

