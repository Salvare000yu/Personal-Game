#pragma once
/// <summary>
/// タイトル画面
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include <memory>
#include "DxBase.h"

class TitleScene :public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void BeforeUpdate();

	void SceneChange();

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;

	bool MoveStartFlag = true;
	float sp = 1.5;
	float StartSp=1.5;

	bool SceneChangeFlag = false;//シーンチェンジ開始前

private:

	std::unique_ptr < Sprite> sprite1 = nullptr;
	std::unique_ptr < Sprite> sp_gametitlename = nullptr;
	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//Model* model_1 = nullptr;
	//Model* model_2 = nullptr;

	//Object3d* object3d_1 = nullptr;
	//Object3d* object3d_2 = nullptr;
	//Object3d* object3d_3 = nullptr;
	int VibCount = 15;//タイトルから何フレーム振動させるか
	int SceneChangeVibCount = 15;//シーンチェンジ中何フレーム振動させるか

	float NamePosXCenter = 0;
	float NamePosYCenter = 0;
};

