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

class TitleScene :public BaseScene
{

public:

	void Initialize() override;

	void Finalize() override;

	void PlayerStandby();//待機中

	void PlayerAppear();//自機の登場

	void NextScene();
	void ToStartSprite();//エンターを押してね!的なUI表示
	void LogoMove();

	//ロゴ動き
	void RightRot();
	void LeftRot();
	void BeforeNextScene();

	void DoorOpen();//扉が開く

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	std::unique_ptr < Player> player_ = nullptr;

private:

	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//----------3dobj

	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Object3d> obj_kaberight = nullptr;

	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	std::unique_ptr < Model> mod_logo = nullptr;//タイトルロゴ
	std::unique_ptr < Object3d> obj_logo = nullptr;//タイトルロゴ

	std::unique_ptr < Model> mod_player = nullptr;// 自機

	float time = 0;
	float frame = 0;

	bool pAppearFlag = true;//自機登場フラグ　true：登場中
	float sp = 1.5;
	float startSp = 1.5;

	bool sceneChangeFlag = false;//シーンチェンジ開始前

	//----------登場退場共通----------//
	float pApMoveFrameMax;//経過時間最大フレーム(登場)
	const int pMoveFrameDef = 0;//経過時間でふぉ
	int pMoveFrame = pMoveFrameDef;//経過時間
	//-----自機登場
	DirectX::XMFLOAT3 playerInitPos;
	DirectX::XMFLOAT3 apStartPPos{};//開始時自機座標
	DirectX::XMFLOAT3 apEndPPos{};//終了
	float camEyeMoveSpX;//カメラ横ずらす値
	//------自機登場
	//------自機退場(シーンチェンジ)
	DirectX::XMFLOAT3 exitStartPPos{};//開始時自機座標
	DirectX::XMFLOAT3 exitEndPPos{};//終了
	float exitPosZ;//ここでおわる
	float sceneChangeDirecPosZ;//シーン遷移演出開始位置
	bool hideTheScreenOnly = false;//一度きり画面隠しフラグ　false：まだしてない
	float pExitMoveFrameMax;//経過時間最大フレーム(退場)
	//------自機退場(シーンチェンジ)

	//------ENTERスプライト点滅
	uint32_t toStartFrameDef;//透明じゃない時間
	uint32_t toStartFrame;
	//-------

	//ロゴの動き　デフォルト
	std::function<void()> logoPattern;

	float logoRotVel = 0;//常にロゴのRotに加算される値

	float RotSp;//回転速度
	float RotMax;//どこまで回転するか
	int16_t PosYSp;//上にずらす値
	int16_t PosYMax;//Y座標の最大値

	int16_t vibCount;//タイトルから何フレーム振動させるか
	int16_t sceneChangeVibCount;//シーンチェンジ中何フレーム振動させるか

	bool doorOpenFlag = false;//扉開けてない

	// カメラ
	std::unique_ptr<CameraTracking> camera;
};
