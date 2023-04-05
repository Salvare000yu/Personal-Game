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
#include "SceneChangeDirection.h"

using namespace DirectX;

class TitleScene :public BaseScene, public BaseObject
{
	//ロゴの動き
	enum class LogoPattern {
		def,
		rightRot,
		leftRot,
		beforeNextScene,//シーン遷移前
	};

	//待機中回転
	enum class StandbyRotPattern {
		def,
		accel,
		deceleration,
		debug,
	};

public:

	void Initialize() override;

	void Finalize() override;

	void PlayerStandby();//待機中

	void PlayerAppear();//自機の登場

	void NextScene();
	void ToStartSprite();//エンターを押してね!的なUI表示
	void LogoMove();

	void DoorOpen();//扉が開く

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;

	bool PAppearFlag = true;//自機登場フラグ　true：登場中
	float sp = 1.5;
	float StartSp = 1.5;

	bool SceneChangeFlag = false;//シーンチェンジ開始前

	////---タイトル常時動く
	//enum class UpDownPattern {
	//	def,
	//	Up,
	//	Down,
	//};
	//UpDownPattern upDownPattern_ = UpDownPattern::def;

	std::unique_ptr < Player> player_ = nullptr;

	float time = 0;
	float frame = 0;

private:

	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//----------3dobj
	std::unique_ptr < Model> mod_tunnel = nullptr;//トンネル
	std::unique_ptr < Object3d> obj_tunnel = nullptr;

	std::unordered_map <std::string, std::unique_ptr < Model>> mod_ground;//地面
	std::unordered_map <std::string, std::unique_ptr < Object3d>> obj_ground;

	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Object3d> obj_kaberight = nullptr;

	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	std::unique_ptr < Model> mod_logo = nullptr;//タイトルロゴ
	std::unique_ptr < Object3d> obj_logo = nullptr;//タイトルロゴ

	std::unique_ptr < Model> mod_player = nullptr;// 自機

	//----------登場退場共通----------//
	const float PApMoveFrameMax = 60.f;//経過時間最大フレーム(登場)
	const float PExitMoveFrameMax = 100.f;//経過時間最大フレーム(退場)
	const int PMoveFrameDef = 0;//経過時間でふぉ
	int PMoveFrame = PMoveFrameDef;//経過時間
	//-----自機登場
	XMFLOAT3 PlayerInitPos{ 0,150,-1950 };
	XMFLOAT3 ApStartPPos{};//開始時自機座標
	XMFLOAT3 ApEndPPos{};//終了
	const float CamEyeMoveSpX = 1.5f;//カメラ横ずらす値
	//------自機登場
	//------自機退場(シーンチェンジ)
	XMFLOAT3 ExitStartPPos{};//開始時自機座標
	XMFLOAT3 ExitEndPPos{};//終了
	const float ExitPosZ = 1300;//ここでおわる
	const float SceneChangeDirecPosZ = -100;//シーン遷移演出開始位置
	bool HideTheScreenOnly = false;//一度きり画面隠しフラグ　false：まだしてない
	//------自機退場(シーンチェンジ)

	//------ENTERスプライト点滅
	const int ToStartFrameDef = 40;//透明じゃない時間
	int ToStartFrame = ToStartFrameDef;
	//-------

	//ロゴの動き　デフォルト
	LogoPattern logoPattern_ = LogoPattern::def;

	float LogoRotVel = 0;//常にロゴのRotに加算される値

	int VibCount = 15;//タイトルから何フレーム振動させるか
	int SceneChangeVibCount = 15;//シーンチェンジ中何フレーム振動させるか

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
