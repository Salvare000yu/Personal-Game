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

	bool pAppearFlag = true;//自機登場フラグ　true：登場中
	float sp = 1.5;
	float startSp = 1.5;

	bool sceneChangeFlag = false;//シーンチェンジ開始前

	std::unique_ptr < Player> player_ = nullptr;

	float time = 0;
	float frame = 0;

private:

	std::unique_ptr < Sprite> sp_titleoper = nullptr;

	//----------3dobj
	std::unique_ptr < Model> mod_tunnel = nullptr;//トンネル
	std::unique_ptr < Object3d> obj_tunnel = nullptr;

	std::unique_ptr < Model> mod_groundBottom = nullptr;//した地面
	std::unique_ptr < Object3d> obj_groundBottom = nullptr;

	std::unordered_map <std::string, std::unique_ptr < Model>> mod_ground;//地面
	std::unordered_map <std::string, std::unique_ptr < Object3d>> obj_ground;

	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Object3d> obj_kaberight = nullptr;

	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	std::unique_ptr < Model> mod_logo = nullptr;//タイトルロゴ
	std::unique_ptr < Object3d> obj_logo = nullptr;//タイトルロゴ

	std::unique_ptr < Model> mod_player = nullptr;// 自機
	//---地面
	//地面二種類を上下に揺らす
	const float swingDist = 10.f;//揺らす距離
	const float swingSp = 2.f;//揺らす速度
	float posDef = -150;//地面を置く座標　ここ中心に上下

	//----------登場退場共通----------//
	const float pApMoveFrameMax = 60.f;//経過時間最大フレーム(登場)
	const float pExitMoveFrameMax = 100.f;//経過時間最大フレーム(退場)
	const int pMoveFrameDef = 0;//経過時間でふぉ
	int pMoveFrame = pMoveFrameDef;//経過時間
	//-----自機登場
	DirectX::XMFLOAT3 playerInitPos{ 0, 150, -1950 };
	DirectX::XMFLOAT3 apStartPPos{};//開始時自機座標
	DirectX::XMFLOAT3 apEndPPos{};//終了
	const float camEyeMoveSpX = 1.5f;//カメラ横ずらす値
	//------自機登場
	//------自機退場(シーンチェンジ)
	DirectX::XMFLOAT3 exitStartPPos{};//開始時自機座標
	DirectX::XMFLOAT3 exitEndPPos{};//終了
	const float exitPosZ = 1300;//ここでおわる
	const float sceneChangeDirecPosZ = -100;//シーン遷移演出開始位置
	bool hideTheScreenOnly = false;//一度きり画面隠しフラグ　false：まだしてない
	//------自機退場(シーンチェンジ)

	//------ENTERスプライト点滅
	const uint32_t toStartFrameDef = 40;//透明じゃない時間
	uint32_t toStartFrame = toStartFrameDef;
	//-------

	//ロゴの動き　デフォルト
	LogoPattern logoPattern_ = LogoPattern::def;

	float logoRotVel = 0;//常にロゴのRotに加算される値

	int vibCount = 15;//タイトルから何フレーム振動させるか
	int sceneChangeVibCount = 15;//シーンチェンジ中何フレーム振動させるか

	bool doorOpenFlag = false;//扉開けてない

	// カメラ
	std::unique_ptr<CameraTracking> camera;
};
