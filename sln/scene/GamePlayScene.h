#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseObject.h"
#include "CameraTracking.h"

#include "Camera.h"

#include "FbxObject3d.h"

#include "Boss.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>

class GamePlayScene :public BaseScene,public BaseObject
{

	DirectX::XMFLOAT2 cameraMoveVel{};
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize() override;

	void Finalize() override;

	void UpdateMouse();
	void UpdateCamera();

	void CollisionAll();

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void SmallEnemyAppear();

	void PlayerDeath();

	void PlayerMove();

	void CoolTime();

	float time;

	//�w��t���[�����ƂɎG���o��
	static const int SEneAppInterval = 80;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//�G
	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//�G���G���X�g���擾
	//const std::list<std::unique_ptr<SmallEnemy>>& GetSmallEnemys() { return smallEnemys_; }

	//�h��鎞��
	static const int32_t pShakeTime = 60/2;
	//�h�ꂽ���܁[
	int32_t pShakeTimer_ = pShakeTime;

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_guide = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbar = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbar = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_semeter = nullptr;

	std::unique_ptr < Model> mod_sword = nullptr;//�f�o�b�N�p�L����
	std::unique_ptr < Model> model_1 = nullptr;//�n��
	std::unique_ptr < Model> mod_worlddome = nullptr;//�V��
	std::unique_ptr < Model> mod_kaberight = nullptr;//��
	std::unique_ptr < Model> mod_kabeleft = nullptr;//��
	std::unique_ptr < Model> mod_smallenemy = nullptr;//�G���G
	std::unique_ptr < Model> mod_playerbullet = nullptr;//���@�e
	std::unique_ptr < Model> mod_enemybullet = nullptr;//�G�e
	std::unique_ptr < Model> mod_player = nullptr;// ���@
	std::unique_ptr < Model> mod_enemy = nullptr;

	std::unique_ptr < Object3d> obj_sword = nullptr;//�f�o�b�N�p�L����
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1 = nullptr;

	// �J����
	std::unique_ptr<CameraTracking> camera;

	float frame = 0;

	//�G���G�o���p�J�E���g
	float SEneAppCount = 0;

	//���@�ʏ�e�З�
	const float pBulPower = 50;
	//�G�ʏ�e�З�
	const float eBulPower = 200;
	//�G���G�ʏ�e�З�
	const float seBulPower = 100;

	//�GHP
	const float BossMaxHP = 1000;//�Ă���������HP 
	float NowBossHP = BossMaxHP;//���݂̓GHP

	//���@HP
	const float PlayerMaxHP = 1000;//������������HP
	float NowPlayerHP = PlayerMaxHP;//���݂̎��@HP
	//�o�[�̂���]�]��������
	bool BarPosControlOnlyOnceFlag1 = false;
	bool BarPosControlOnlyOnceFlag2 = false;
	bool BarPosControlOnlyOnceFlag3 = false;
	bool BarPosControlOnlyOnceFlag4 = false;
	bool BarPosControlOnlyOnceFlag5 = false;
	bool BarPosControlOnlyOnceFlag6 = false;
	bool BarPosControlOnlyOnceFlag7 = false;
	//���@�_���[�W�t���O �����ĂȂ�
	bool pDamFlag = false;
	//�G���j��
	float sEnemyMurdersNum = 0;
	//�{�X��܂ł̓G�E�Q�K�v��
	float BossTermsEMurdersNum = 5;
	//�{�X�o�������B���I
	bool BossEnemyAdvent = false;
	
	int randShakeDef = 0;
	int randShakeNow=randShakeDef;
};

