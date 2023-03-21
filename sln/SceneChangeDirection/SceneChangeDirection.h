#pragma once
#include "Sprite.h"
#include "BaseObject.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include "DxBase.h"
using namespace DirectX;

/// <summary>
/// シーン遷移時の演出
/// </summary>

class SceneChangeDirection
{
public:
	static SceneChangeDirection* GetInstance();
	void Initialize();

	void HideTheScreen();

	void Update();

	void Draw();

	//シーン遷移フラグ　false:してない
	bool SceneChangeDirectionFlag = false;
	//シーン遷移完了　false:完了前
	bool SceneChangeCompFlag = false;
	//画面隠す　false:隠す前
	bool HideTheScreenFlag = false;

private:

	std::unique_ptr < Sprite> sp_scenechange = nullptr;

	const float HideSp = 10;//画面隠す速度

	float HideVel = 0;//実際に座標に足す値

};