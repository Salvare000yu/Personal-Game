#pragma once
#include "Sprite.h"
#include "BaseObject.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include "DxBase.h"
using namespace DirectX;

/// <summary>
/// ƒV[ƒ“‘JˆÚ‚Ì‰‰o
/// </summary>

class SceneChange
{
public:
	static SceneChange* GetInstance();
	void Initialize();

	void Update();

	void Draw();

private:

	std::unique_ptr < Sprite> sp_scenechange = nullptr;

};