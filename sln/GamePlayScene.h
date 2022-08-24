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

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	//指定フレームごとに雑魚出現
	static const int SEneAppInterval = 50;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;

	//敵
	Enemy* enemy_ = nullptr;
	Player* player_ = nullptr;
	SmallEnemy* smallEnemy_ = nullptr;

	//衝突判定と応答
	void CheckAllCollisions();

private:

	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_guide = nullptr;

	std::unique_ptr < Model> mod_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Model> model_1 = nullptr;//地面
	std::unique_ptr < Model> mod_worlddome = nullptr;//天球

	std::unique_ptr < Object3d> obj_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1=nullptr;

	float frame = 0;

	//雑魚敵出現用カウント
	float SEneAppCount = 0;
};

