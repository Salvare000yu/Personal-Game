#pragma once
/// <summary>
/// ƒ^ƒCƒgƒ‹‰æ–Ê
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

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;
private:

	std::unique_ptr < Sprite> sprite1 = nullptr;

	//Model* model_1 = nullptr;
	//Model* model_2 = nullptr;

	//Object3d* object3d_1 = nullptr;
	//Object3d* object3d_2 = nullptr;
	//Object3d* object3d_3 = nullptr;
};

