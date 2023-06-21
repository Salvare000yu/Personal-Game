/// <<<<<<<<
/// @brief ボスのクラス
/// >>>>>>>>

#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BossBullet.h"
#include "BossAimBul.h"
#include "BossStraightBul.h"
#include "BaseObject.h"
#include "ParticleManager.h"

#include <memory>
#include <functional>

class Boss :public BaseObject
{
public:

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//シェイク
	void Shake();

	//攻撃
	void Attack();

	void PAimBul();
	void BossAppear();
	void Approach();
	void Leave();

	void Vertical();
	void StartVertical();
	void VerticalWait();
	void VerticalDown();
	void VerticalUp();
	void VerticalReverse();
	void EndVertical();

	void HpHalfPatStart();
	void CircularMotionMove();
	void LeaveFirstPos();
	void PlungeInto();
	void PlungeIntoLeave();
	void PlungeIntoWait();
	void Plunge();
	void PlungeIntoReverse();
	void AfterPlungeInto();
	void AfterPlungeWait();
	void AfterPlungeAttack();
	void AfterPlungeFin();
	//拡散攻撃
	void DiffusionAttack();
	//拡散偶数弾
	void DiffusionAttackEavenNumber();
	//直線攻撃
	void StraightAttack();
	//くらい演出　体赤く
	void DamageEffect();

	void Death();

	void AlwaysmMotion();

	//近づく処理初期化
	void ApproachInit();

	//-----------------↓げったーせったー↓------------------//
	//弾リストを取得
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }
	const std::list<std::unique_ptr<BossAimBul>>& GetAimBullets() { return aimBullets_; }
	const std::list<std::unique_ptr<BossStraightBul>>& GetStraightBullets() { return straightBullets_; }

	inline void SetBulModel(Model* model) { bulModel = model; }
	inline void SetAimBulModel(Model* model) { aimBulModel = model; }
	inline void SetStraightBulModel(Model* model) { straightBulModel = model; }

	//通常弾威力
	const float& GetBulPow() { return bulPow; }
	//狙い弾威力
	const float& GetAimBulPow() { return aimBulPow; }
	//直線威力
	const float& GetStraightBulPow() { return straightBulPow; }
	//ボス本体にぶつかった時の威力
	const int& GetBodyPow() { return bodyPow; }

	//ボス生死
	void SetisDeath(bool isDeath) { this->isDeath = isDeath; }
	const bool& GetisDeath() { return isDeath; }

	//ボス当たり判定するか
	void SetDoCollision(bool doCollision) { this->doCollision = doCollision; }
	const bool& GetDoCollision() { return doCollision; }

	//狙い弾　打つ相手
	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }

	void SetBossDamageEffect(bool bossDamageEffectFlag) {this->bossDamageEffectFlag = bossDamageEffectFlag;}
	//-----------------↑げったーせったー↑------------------//

private:
	std::unique_ptr < Model> mod_core = nullptr;
	std::unique_ptr < Model> mod_AroundCore = nullptr;
	std::unique_ptr < Model> mod_outside = nullptr;
	std::unique_ptr < Model> mod_SideSquare = nullptr;
	std::unique_ptr < Model> mod_UpDown = nullptr;
	std::unique_ptr < Model> mod_VerticalCircle = nullptr;

	std::unique_ptr < Object3d> obj_core = nullptr;
	std::unique_ptr < Object3d> obj_AroundCore = nullptr;
	std::unique_ptr < Object3d> obj_outside = nullptr;
	std::unique_ptr < Object3d> obj_SideSquare = nullptr;
	std::unique_ptr < Object3d> obj_UpDown = nullptr;
	std::unique_ptr < Object3d> obj_VerticalCircle = nullptr;

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//パーティクル
	std::unique_ptr< ParticleManager> particle;

	std::unique_ptr<Camera> camera; //カメラ

	//離れられる距離
	float leaveLim = 90;

	bool bossDamageEffectFlag = false;//ダメージ演出　false:やってない

	//フレームごとに発射
	static const uint8_t atkInterval = 10;
	static const uint8_t atkInterval_LeaveFirst = 20;
	static const uint8_t diffusionAtkInterval = 20;

	//フレーム
	float frame = 0;

	//フレーム計測
	const int nowframeDef = 0;
	int nowframe = nowframeDef;

	//当たり判定取るか true:取る
	bool doCollision = true;

	//ボス回転
	float bossRot = 1.f;

	//ボスlerp動きレート
	float bossLerpMoveRaito = 0;

	//攻撃用カウント
	uint8_t atkCount = 0;
	uint8_t diffusionAtkCount = 0;
	uint8_t circular_AtkCount = 0;	//ぐるぐる時攻撃用カウント
	uint8_t afterPlungePatAtkCount = 10;//突っ込み後行動の攻撃カウント

	bool even_odd_NumFlag = true;//奇数弾

	Model* bulModel = nullptr;
	Model* aimBulModel = nullptr;
	Model* straightBulModel = nullptr;

	//弾威力
	const float bulPow = 200;
	const float aimBulPow = 150;
	const float straightBulPow = 170;

	//ボス本体の威力
	const int bodyPow = 250;

	//false：まだ死んでない
	bool isDeath = false;

	//死んだとき一度
	bool isFirst_Death = false;

	XMFLOAT3 boPosDeath = {};
	bool getPosOnlyFlag = true;//一度きりの座標読み込み
	bool getPosDeathOnlyFlag = true;//一度きりの座標読み込み
	const float necesFrame = 190.0f;//かかる時間
	XMFLOAT3 moveSp = {};//移動速度
	XMFLOAT3 targetPos = { 0,-150,0 };//目標座標
	float coreColChangeRaito = 0;//死亡時コアの色変え

	uint8_t partTimeInterval;
	uint8_t particleFrame = 39;//パーティクル出すフレ

	//縦攻撃
	std::function<void()> verticalPattern;
	//一度離れてから突っ込む行動パターン　最初離れる
	std::function<void()> plungeIntoPattern;
	//突っ込み後の行動パターン 最初待ち
	std::function<void()> afterPlungePattern;

	std::function<void()> actionPattern;

	std::list <std::unique_ptr<BossBullet>> bullets_;//ボスの弾　ユニークポインタ
	std::list <std::unique_ptr<BossAimBul>> aimBullets_;//ボスの狙い弾
	std::list <std::unique_ptr<BossStraightBul>> straightBullets_;//ボスの直線弾

	float time;

	//近づく速さ
	const float approachSpZ = 3.f;
	const float approachSpY = 2.f;
	//離れる速さ
	const float leaveSpZ = 3.f;
	const float leaveSpY = 2.f;

	//この時間だけこの行動をする
	const uint16_t approachCountDef = 150;
	uint16_t approachCount = approachCountDef;

	//------縦に揺れる攻撃F
	const int changeVerticalCountDef = 0;//デフォ0　終わったら消してん
	int changeVerticalCount = changeVerticalCountDef;
	const int changeVerticalNeces = 3;//縦攻撃に移る為に必要カウント
	//最初の上昇値
	const int startVerticalValDef = -5;
	int startVerticalVal = startVerticalValDef;
	//上昇下降速度
	float verticalSp = 15;
	//待ち時間
	const int verticalWaitCountDef = 50;
	int verticalWaitCount = verticalWaitCountDef;
	const XMFLOAT3 upDownPosDef = { -600,900,0 };	//上昇下降開始座標
	XMFLOAT3 upDownPos = upDownPosDef;
	float upStartPosY = -350;//上昇開始Y座標
	float downStartPosY = 1000;//下降開始Y座標
	const float nextMoveX = 200;//UpDownの最後にXをずらす値
	//UpDown時の最初に開始位置決めるフラグ false:決める前
	bool verticalStartPosFlag = false;
	//次の動き false:これじゃない
	bool nextUp = false;
	bool nextDown = true;//最初は下降
	//UpDown何回ループさせるか
	const int verticalLoopCountDef = 4;
	int verticalLoopCount = verticalLoopCountDef;
	//戻る行動開始座標に移動
	XMFLOAT3 reverseStartPos;
	//戻る行動開始座標を最初に一回やる　false:まだ
	bool reverseStartPosFlag = false;
	//縦攻撃用インターバル
	static const int vertical_AtkInterval = 7;

	//------

	//-------↓HPHALF↓------//
	bool isHpHalfPattern = false;//hp半分以下行動してない

	XMFLOAT3 hpHalfMomentPos = {};
	//まずこの位置に行く
	XMFLOAT3 targetHpHalfPos;
	const float necesHpHalfFrame = 100.0f;//HP半分時このフレーム分移動まで時間かかる

	const float necesLeaveFirstFrame = 180.f;

	XMFLOAT4 coreCol{0,0,1,1};//コアの色　最初青
	//-------↑HPHALF↑------//
	
	//------HP半分以下円運動↓
	const float hpHalf_AngleDef = 20.f;
	float hpHalf_Angle = hpHalf_AngleDef;
	const float hpHalf_radDef = 100.f;
	float hpHalf_rad = hpHalf_radDef;

	const float hpHalf_LengthDef = 10.f;//円運動の半径の長さ
	float hpHalf_Length = hpHalf_LengthDef;

	const float circularY = 40;//どの高さで回るか

	XMFLOAT3 circlePosMem;//その時の座標取得
	//移動値
	const float addXDef = 0.f;
	float addX = addXDef;
	const float addYDef = 0.f;
	float addY = addYDef;
	//------HP半分以下円運動↑

	static const int circular_AtkIntervalDef = 45;
	int circular_AtkInterval = circular_AtkIntervalDef;

	//突っ込み後行動のAimBulインターバル
	static const int afterPlungePatAtkInterval = 15;

	BaseObject* shotTag;//弾うつターゲット

	//登場から行動を開始に移った時の座標
	XMFLOAT3 actionStartPos;

	//-------PlungeInto
	//2回Leaveをしたら突っ込む行動　　デフォ2
	const int plungeCountDef = 1;
	int plungeCount = plungeCountDef;
	//突っ込み行動へ移行する前に最後にいた場所を記憶する
	XMFLOAT3 wasPosMem;
	//離れる速度
	const int leaveVel = 40;
	//Leaveの時どの程度下がるか
	const int leavePos = 6000;
	//戻る前の座標取得フラグ　false:してない
	bool beforeReversePosMemFlag = false;
	//戻る前座標入れる
	XMFLOAT3 beforeReversePosMem;
	//今の場所
	XMFLOAT3 reverseNowPos;
	//どこに戻るかの場所
	XMFLOAT3 reverseTargetPos;
	//突っ込み待ち待機時間 でふぉ140
	const int plungeIntoWaitCountDef = 140;
	int plungeIntoWaitCount = plungeIntoWaitCountDef;
	//最後定位置戻るのにかかる時間
	const int plungeFinFrameMax = 60;
	//Finで最初いた一度位置取得 false:まだ
	bool plungeFinOnlyFlag = false;

	//----plungeとafter両立しないのでafterでも使う
	//自機いた場所取得フラグ false:未取得
	bool pMemFlag = false;
	//自機いた場所
	XMFLOAT3 pPosMem{};

	//突っ込み終わったか  false:まだ突っ込んでない
	bool plungeCompletFlag = false;
	//ボス最初の場所
	bool boPosFlag = false;
	//ボスいた場所
	XMFLOAT3 boPosMom{};
	//突っ込み速度
	const uint32_t plungeNecesFrame = 40;
	//その時のボスの位置
	XMFLOAT3 plungeNowPos;
	//-------PlungeInto

	//-------AfterPlungeInto
	//
	//待ち時間
	const uint8_t waitTimeDef = 60;
	uint8_t waitTime = waitTimeDef;
	//攻撃時に自機のいた場所へ（XY）移動する速度
	XMFLOAT3 atkMoveSp;
	//ボスいた場所
	XMFLOAT3 boPosMem;
	//ボスの現在地
	XMFLOAT3 boNowPos;
	//攻撃に移動にかかる絶対時間
	const int necesAtkMoveTime = 60;
	//WaitとAttack繰り返した回数
	const int loopCountDef = 0;
	int loopCount = loopCountDef;
	//この回数でつぎへ
	const int loopCountMax = 7;
	//
	//-------AfterPlungeInto

	//<<<<<<シェイク
	int randShakeDef = 0;
	int randShakeNow = randShakeDef;
	//いた場所保存 false:保存前
	bool shakePosMemFlag = false;
	//揺らし終えたか false:まだ
	bool isShakeFinished = false;
	//いた場所
	XMFLOAT3 posMem = {};
	//>>>>>>シェイク

	//<<<<<<ダメージ受けた時体赤くする演出
	const uint8_t bossBodyRedTimeDef = 10;
	uint8_t bossBodyRedTime = bossBodyRedTimeDef;
	//赤から戻していく色
	float reCol = 0.f;//赤状態
	float reColVal = 0.05f;//戻す数値
	//>>>>>>ダメージ受けた時体赤くする演出
};