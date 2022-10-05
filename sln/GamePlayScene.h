#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseObject.h"


#include "Camera.h"

#include "FbxObject3d.h"

#include "Boss.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>

class GamePlayScene :public BaseScene,public BaseObject
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void SmallEnemyAppear();

	void Obj2move();

	std::unique_ptr<Camera> camera; //�J����

	float time;

	//�w��t���[�����ƂɎG���o��
	static const int SEneAppInterval = 50;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//�G
	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//�G���G���X�g���擾
	//const std::list<std::unique_ptr<SmallEnemy>>& GetSmallEnemys() { return smallEnemys_; }

	//�h��鎞��
	static const int32_t pShakeTime = 60*3;
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

	float frame = 0;

	//�G���G�o���p�J�E���g
	float SEneAppCount = 0;

	//���@�ʏ�e�З�
	const float pBulPower = 50;
	//�G�ʏ�e�З�
	const float eBulPower = 200;

	//�GHP
	const int BossMaxHP = 1000;//�Ă���������HP 
	int NowBossHP = BossMaxHP;//���݂̓GHP

	//���@HP
	const int PlayerMaxHP = 1000;//������������HP
	int NowPlayerHP = PlayerMaxHP;//���݂̎��@HP
	bool BarPosControlOnlyOnceFlag1 = false;
	bool BarPosControlOnlyOnceFlag2 = false;
	//���@�_���[�W�t���O �����ĂȂ�
	bool pDamFlag = false;
	//�G���j��
	int sEnemyMurdersNum = 0;
	//�{�X��܂ł̓G�E�Q�K�v��
	int BossTermsEMurdersNum = 8;
	//�{�X�o�������B���I
	bool BossEnemyAdvent = false;
	
};

