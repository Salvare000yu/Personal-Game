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
#include <unordered_map>

using namespace DirectX;

class GamePlayScene :public BaseScene, public BaseObject
{

	DirectX::XMFLOAT2 cameraMoveVel{};

	enum class BeforeBossPattern {
		def,
		inc,
		dec,
	};

	enum class PlayerDashDirection {
		def,
		right,
		left,
		up,
		down,
	};

private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	void Initialize() override;

	void Finalize() override;

	void UpdateMouse();
	void UpdateCamera();

	void PadStickCamera();

	void CollisionAll();//����

	bool GameReady();//�J�n���}

	void BodyDamCoolTime();//�̃_���[�W�N�[���^�C��

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void PlayTimer();

	void SmallEnemyCreate();	//�G���G����
	void SmallEnemyAppear();	//�G���G�o��

	void BossConditionComp();//�{�X�������������

	void DoorOpen();//�����J��
	void pHeadingToTheNextPlace();//���̏ꏊ�֍s��
	void BeforeBossAppear();

	void BossDeathEffect();

	void PlayerMove();
	void PlayerDash();//�_�b�V��

	void CoolTime();

	float time = 0;

	//�w��t���[�����ƂɎG���o��
	static const int SEneAppInterval = 60;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;
	std::unique_ptr < PlayerFireLine> firingline_ = nullptr;

	//�h��鎞��
	static const int32_t pShakeTime = 60 / 2;
	//�h�ꂽ���܁[
	int32_t pShakeTimer_ = pShakeTime;

	//���@�_���[�W�t���O �����ĂȂ�
	bool pDamFlag = false;

	std::vector<XMVECTOR> points;
	size_t splineStartIndex;

	BeforeBossPattern beforeBossPattern_ = BeforeBossPattern::dec;

	//�_�b�V���������
	PlayerDashDirection playerDashDirection_ = PlayerDashDirection::def;

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_sight = nullptr;
	std::unique_ptr < Sprite> sp_beforeboss = nullptr;
	std::unique_ptr < Sprite> sp_ready = nullptr;
	std::unique_ptr < Sprite> sp_ready_go = nullptr;
	std::unique_ptr < Sprite> sp_blackwindow = nullptr;

	std::unique_ptr < Model> mod_ground = nullptr;//�n��
	std::unique_ptr < Model> mod_kaberight = nullptr;//��
	std::unique_ptr < Model> mod_kabeleft = nullptr;//��
	std::unique_ptr < Model> mod_smallenemy = nullptr;//�G���G
	std::unique_ptr < Model> mod_playerbullet = nullptr;//���@�e
	std::unique_ptr < Model> mod_enemybullet = nullptr;//�G�e
	std::unique_ptr < Model> mod_bossaimbullet = nullptr;//�G�_���e
	std::unique_ptr < Model> mod_straightbul = nullptr;//�����e
	std::unique_ptr < Model> mod_player = nullptr;// ���@
	std::unique_ptr < Model> mod_enemy = nullptr;
	std::unique_ptr < Model> mod_firingline = nullptr;
	std::unique_ptr < Model> mod_tunnel = nullptr;//�g���l��
	std::unique_ptr < Model> mod_backwall = nullptr;//���Ō�̕�

	std::unordered_map<std::string, std::unique_ptr < Object3d>> obj_ground;
	std::unique_ptr < Object3d> obj_ground = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;
	std::unique_ptr < Object3d> obj_tunnel = nullptr;
	std::unique_ptr < Object3d> obj_backwall = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1 = nullptr;

	// �J����
	std::unique_ptr<CameraTracking> camera;

	//----�|�X�g�G�t�F�N�g�֘A
	//���B�l�b�g����
	float vignettePow = 0;

	//-----�J�n�����o
	//�Q�[���J�n���t���[��
	int GameReadyFrame = 0;
	//�J�n�����@���W
	XMFLOAT3 ApStartPPos{};
	//�I��
	XMFLOAT3 ApEndPPos{};
	//�Ǐ]�J������񂾂��Z�b�g
	bool SetTagOnceFlag = false;

	//GO�������t���O
	bool ready_GOFlag = false;//false��\��
	//��x����U�����o�� false:�܂������ĂȂ�
	bool MayDoPAtk_OnceFlag = false;

	//-----�J�n�����o

	float frame = 0;

	//�G���G�o���p�J�E���g
	float SEneAppCount = 0;

	//�G���j��
	float sEnemyMurdersNum = 0;
	//�{�X��܂ł̓G�E�Q�K�v�� 10
	float BossTermsEMurdersNum = 0;
	//�{�X�o�������B���I
	bool BossEnemyAdvent = false;
	//�{�X�o���O���o�t���O
	bool BeforeBossAppearFlag = false;
	//true:������Ă�
	bool BeforeBossAppearNow = false;

	//�{�X�̑̂ƏՓ˃_���[�W�N�[���^�C�� false:��炤�O
	bool BodyDamFlag = false;
	//���̃N�[���^�C���J�E���g
	const int BodyDamCountDef = 30;
	int BodyDamCount = BodyDamCountDef;

	int randShakeDef = 0;
	int randShakeNow = randShakeDef;

	int BBPaternCount = 0;//1��0��++

	bool AlertSoundFlag = true;//�x�����J��Ԃ��Ɉ��̂�

	bool pRotDef = false;//�J�����ŏ��Ƀ}�E�X�̏ꏊ�ŃY�����Ⴄ�����x���ʂɌ�����

	//���摜����������l
	const float colordec = 0.006f;

	//�_���[�W��ʒ[�Ԃ��@false�F�܂�����ĂȂ�
	bool DamEfRedFlag = false;

	//���@�����ȃt���O true:�� �� �ȁ@false:�����Ă悢
	bool PDontMoveFlag = true;
	//���@�ړ������ǂ��� false:���ĂȂ�
	bool isLMove = false;
	bool isRMove = false;

	bool DoorOpenFlag = false;//���J���ĂȂ�

	//���@���S���o�����񎩋@���W�擾�@false:����Ă��Ȃ�
	bool PDeathDirection_PosMemOnlyFlag = false;

	//�֐��n�܂����Ƃ��̂� false:�܂�
	bool PDeathEffect_PosMemOnlyFlag = false;

	//���̏ꏊ�֍s���X�s�[�h
	const float pNextPlaceGoSpMax = 10.f;
	float pNextPlaceGoSp = 2.f;
	float AccelVal = 0.1f;//�����l
	float DecelVal = 0.2f;//�����l

	//�^����З�
	float Damage{};

	std::vector<std::vector<std::string>> csvData;
	//���s�ڂ܂ŏo������
	int seIndex = -1;

	//----���@�_�b�V�� 
	//false:���ĂȂ�
	bool DashFlag = false;
	//�_�b�V������
	const int DashCountDef = 30;
	int DashCount = DashCountDef;
	//�_�b�V���J�E���g�����̕��������l�ɂȂ����猸��
	const int DashAttenuation = 10;
	bool DashAttenuationFlag = false;//�����J�n fasle:�܂����ĂȂ�
	//�������l
	float Attenuation = -0.2f;
	//�_�b�V���N�[���C���^�[�o��
	const int DashIntervalDef = 40;
	int DashInterval = DashIntervalDef;
	bool DashIntervalFlag = false;//false:�v���O true:�_�b�V���ł��Ȃ���
	//�_�b�V�����x
	XMFLOAT3 DashVel = { 0,0,0 };
	//���ۂɑ��₷�l
	const float DashVelIncDef = 6;
	float DashVelInc = DashVelIncDef;

	//----���@�_�b�V�� 
};

