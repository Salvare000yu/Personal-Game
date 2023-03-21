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

	void OpenTheScreen();

	void Update();

	void Draw();

	//シーン遷移フラグ　false:してない
	bool SceneChangeDirectionFlag = false;
	//シーン遷移完了　false:完了前
	bool SceneChangeCompFlag = false;
	//画面隠す　false:隠し始める前
	bool HideTheScreenFlag = false;
	//playsceneが始まり演出始める　false:まだ
	bool GameReadyStartFlag = false;

private:

	std::unique_ptr < Sprite> sp_scenechange = nullptr;

	const float HideSp = 40;//画面隠す速度 画像サイズで割れる大きさ

	float HideVel = 0;//実際に座標に足す値

};