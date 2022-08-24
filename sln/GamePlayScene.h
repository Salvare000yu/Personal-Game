#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"

#include "FbxObject3d.h"

#include "Enemy.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>

class GamePlayScene :public BaseScene
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

	//�G
	Enemy* enemy_ = nullptr;
	Player* player_ = nullptr;
	SmallEnemy* smallEnemy_ = nullptr;

	//�Փ˔���Ɖ���
	void CheckAllCollisions();

private:

	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_guide = nullptr;

	std::unique_ptr < Model> mod_sword = nullptr;//�f�o�b�N�p�L����
	std::unique_ptr < Model> model_1 = nullptr;//�n��
	std::unique_ptr < Model> mod_worlddome = nullptr;//�V��

	std::unique_ptr < Object3d> obj_sword = nullptr;//�f�o�b�N�p�L����
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1=nullptr;

	float frame = 0;

	//�G���G�o���p�J�E���g
	float SEneAppCount = 0;
};

