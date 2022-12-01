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
#include "PlayerFireLine.h"

#include <memory>

using namespace DirectX;

class GamePlayScene :public BaseScene,public BaseObject
{

	DirectX::XMFLOAT2 cameraMoveVel{};

	enum class BeforeBossPattern {
		def,
		inc,
		dec,
	};

private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//// DirectX::���ȗ�
	//using XMFLOAT2 = DirectX::XMFLOAT2;
	//using XMFLOAT3 = DirectX::XMFLOAT3;
	//using XMFLOAT4 = DirectX::XMFLOAT4;
	//using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize() override;

	void Finalize() override;

	void UpdateMouse();
	void UpdateCamera();

	void PadStickCamera();

	void CollisionAll();

	bool GameReady();

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void PlayTimer();

	void SmallEnemyAppear();

	void BeforeBossAppear();

	void BossDeathEfect();

	void PlayerMove();

	void CoolTime();

	XMVECTOR SplinePosition(const std::vector<XMVECTOR>& posints, size_t startIndex, float t);

	float time;

	//�w��t���[�����ƂɎG���o��
	static const int SEneAppInterval = 80;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//�G���G���X�g���擾
	//const std::list<std::unique_ptr<SmallEnemy>>& GetSmallEnemys() { return smallEnemys_; }

	//�h��鎞��
	static const int32_t pShakeTime = 60/2;
	//�h�ꂽ���܁[
	int32_t pShakeTimer_ = pShakeTime;

	//���@�_���[�W�t���O �����ĂȂ�
	bool pDamFlag = false;

	std::vector<XMVECTOR> points;
	size_t splineStartIndex;

	BeforeBossPattern beforeBossPattern_ = BeforeBossPattern::dec;

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	//std::unique_ptr < Sprite> sp_guide = nullptr;
	std::unique_ptr < Sprite> sp_sight = nullptr;
	std::unique_ptr < Sprite> sp_beforeboss = nullptr;
	std::unique_ptr < Sprite> sp_ready = nullptr;
	std::unique_ptr < Sprite> sp_ready_go = nullptr;
	std::unique_ptr < Sprite> sp_blackwindow = nullptr;
	std::unique_ptr < Sprite> sp_dame_ef = nullptr;

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
	std::unique_ptr < Model> mod_firingline = nullptr;
	std::unique_ptr < Model> mod_tunnel = nullptr;//�g���l��

	std::unique_ptr < Object3d> obj_sword = nullptr;//�f�o�b�N�p�L����
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;
	std::unique_ptr < Object3d> obj_tunnel = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1 = nullptr;

	// �J����
	std::unique_ptr<CameraTracking> camera;

	//GO�������t���O
	bool ready_GOFlag = false;//false��\��

	float frame = 0;

	//�G���G�o���p�J�E���g
	float SEneAppCount = 0;

	//�G�ʏ�e�З�
	const float eBulPower = 200;

	//�G���j��
	float sEnemyMurdersNum = 0;
	//�{�X��܂ł̓G�E�Q�K�v��
	float BossTermsEMurdersNum = 3;
	//�{�X�o�������B���I
	bool BossEnemyAdvent = false;
	//�{�X�o���O���o�t���O
	bool BeforeBossAppearFlag = false;
	//true:������Ă�
	bool BeforeBossAppearNow = false;
	
	int randShakeDef = 0;
	int randShakeNow=randShakeDef;

	int BBPaternCount = 0;//1��0��++

	bool AlertSoundFlag = true;//�x�����J��Ԃ��Ɉ��̂�

	bool pRotDef = false;//�J�����ŏ��Ƀ}�E�X�̏ꏊ�ŃY�����Ⴄ�����x���ʂɌ�����

	//���摜����������l
	const float colordec = 0.006;

	//�_���[�W��ʒ[�Ԃ��@false�F�܂�����ĂȂ�
	bool DamEfRedFlag = false;

	//���@�ړ������ǂ��� false:���ĂȂ�
	bool isLMove = false;
	bool isRMove = false;
};

