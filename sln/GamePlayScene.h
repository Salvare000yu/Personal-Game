#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"

#include "FbxObject3d.h"

#include "Enemy.h"
#include "Player.h"

#include <memory>

class GamePlayScene :public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void Obj2move();

	std::unique_ptr<Camera> camera; //ÉJÉÅÉâ

	float time;

	//ìG
	Enemy* enemy_ = nullptr;
	Player* player_ = nullptr;

private:

	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_guide = nullptr;

	std::unique_ptr < Model> model_1 = nullptr;//ínñ 
	std::unique_ptr < Model> mod_worlddome = nullptr;//ìVãÖ

	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1=nullptr;

	float frame = 0;
};

