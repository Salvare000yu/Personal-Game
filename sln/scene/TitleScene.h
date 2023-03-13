#pragma once
/// <summary>
/// タイトル画面
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "BaseObject.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include "Player.h"
#include <memory>
#include "DxBase.h"
#include "CameraTracking.h"
#include "Camera.h"

using namespace DirectX;

class TitleScene :public BaseScene, public BaseObject
{

public:

	void Initialize() override;

	void Finalize() override;

	void BeforeUpdate();

	void PlayerAppear();//自機の登場

	void SceneChange();

	void UpDown();

	void DoorOpen();//扉が開く

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;

	bool MoveStartFlag = true;
	float sp = 1.5;
	float StartSp=1.5;

	bool SceneChangeFlag = false;//シーンチェンジ開始前

	////---タイトル常時動く
	//enum class UpDownPattern {
	//	def,
	//	Up,
	//	Down,
	//};
	//UpDownPattern upDownPattern_ = UpDownPattern::def;

	std::unique_ptr < Player> player_ = nullptr;

	float time;
	float frame = 0;

private:

	std::unique_ptr < Sprite> sprite1 = nullptr;
	std::unique_ptr < Sprite> sp_gametitlename = nullptr;
	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//----------3dobj
	std::unique_ptr < Model> mod_tunnel = nullptr;//トンネル
	std::unique_ptr < Model> mod_ground = nullptr;//地面
	std::unique_ptr < Model> mod_player = nullptr;// 自機
	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁

	std::unique_ptr < Object3d> obj_tunnel = nullptr;
	std::unique_ptr < Object3d> obj_ground = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	//-----自機登場
	int PAppearFrame = 0;
	//開始時自機座標
	XMFLOAT3 ApStartPPos;
	//終了
	XMFLOAT3 ApEndPPos;
	//------

	int VibCount = 15;//タイトルから何フレーム振動させるか
	int SceneChangeVibCount = 15;//シーンチェンジ中何フレーム振動させるか

	float NamePosXCenter = 0;
	float NamePosYCenter = 0;

	bool DoorOpenFlag = false;//扉開けてない

	// カメラ
	std::unique_ptr<CameraTracking> camera;

	////これ超えたらパターン変える　最大最小
	//const float NamePosMoveMax = 7;
	//const float NamePosMoveMin = -7;
	////上げ下げする値
	//const float NamePosYUpDownDef = 0.2;//デフォ
	//float NamePosYUpDown = NamePosYUpDownDef;
};

