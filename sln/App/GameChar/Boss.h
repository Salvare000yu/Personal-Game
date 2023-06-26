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

	//-----------------↓げったーせったー↓------------------//
	//弾リストを取得
	const std::forward_list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }
	const std::forward_list<std::unique_ptr<BossAimBul>>& GetAimBullets() { return aimBullets_; }
	const std::forward_list<std::unique_ptr<BossStraightBul>>& GetStraightBullets() { return straightBullets_; }

	inline void SetBulModel(Model* model) { bulModel = model; }
	inline void SetAimBulModel(Model* model) { aimBulModel = model; }
	inline void SetStraightBulModel(Model* model) { straightBulModel = model; }

	//通常弾威力
	const int32_t& GetBulPow() { return bulPow; }
	//狙い弾威力
	const int32_t& GetAimBulPow() { return aimBulPow; }
	//直線威力
	const int32_t& GetStraightBulPow() { return straightBulPow; }
	//ボス本体にぶつかった時の威力
	const int32_t& GetBodyPow() { return bodyPow; }

	//ボス防御力
	void SetBossDefense(int32_t bossDefense) { this->bossDefense = bossDefense; }
	const int32_t& GetBossDefense() { return bossDefense; }

	//現在ボスHP
	void SetNowBoHp(int32_t nowBossHP) { this->nowBossHP = nowBossHP; }
	const int32_t& GetNowBoHp() { return nowBossHP; }
	//ボスHPMax
	const int32_t& GetBoMaxHp() { return bossMaxHP; }

	//ボス生死
	void SetisDeath(bool isDeath) { this->isDeath = isDeath; }
	const bool& GetisDeath() { return isDeath; }

	//ボス当たり判定するか
	void SetDoCollision(bool doCollision) { this->doCollision = doCollision; }
	const bool& GetDoCollision() { return doCollision; }

	//ボス戦中か
	void SetBossEnemyAdvent(bool bossEnemyAdvent) { this->bossEnemyAdvent = bossEnemyAdvent; }
	const bool& GetBossEnemyAdvent() { return bossEnemyAdvent; }

	//狙い弾　打つ相手
	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }

	void SetBossDamageEffect(bool bossDamageEffectFlag) { this->bossDamageEffectFlag = bossDamageEffectFlag; }
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
	float leaveLim;

	bool bossDamageEffectFlag = false;//ダメージ演出　false:やってない

	//フレームごとに発射
	uint32_t atkInterval;
	uint32_t atkInterval_LeaveFirst;
	uint32_t diffusionAtkInterval;

	//フレーム
	float frame = 0;

	//フレーム計測
	const int nowframeDef = 0;
	int nowframe = nowframeDef;

	//当たり判定取るか true:取る
	bool doCollision = true;

	//ボス回転
	float bossRot;

	//ボスlerp動きレート
	float bossLerpMoveRaito = 0;

	//攻撃用カウント
	uint32_t atkCount = 0;
	uint32_t diffusionAtkCount = 0;
	uint32_t circular_AtkCount = 0;	//ぐるぐる時攻撃用カウント
	uint32_t afterPlungePatAtkCount;//突っ込み後行動の攻撃カウント

	bool even_odd_NumFlag = true;//奇数弾

	Model* bulModel = nullptr;
	Model* aimBulModel = nullptr;
	Model* straightBulModel = nullptr;

	//弾威力
	int32_t bulPow;
	int32_t aimBulPow;
	int32_t straightBulPow;

	//ボス本体の威力
	int32_t bodyPow;

	//防御力
	int32_t bossDefenseDef;
	int32_t bossDefense;
	int32_t haHalfDefence;

	//false：まだ死んでない
	bool isDeath = false;

	//死んだとき一度
	bool isFirst_Death = false;

	XMFLOAT3 boPosDeath = {};
	bool getPosOnlyFlag = true;//一度きりの座標読み込み
	bool getPosDeathOnlyFlag = true;//一度きりの座標読み込み
	float necesFrame;//かかる時間
	XMFLOAT3 moveSp = {};//移動速度
	XMFLOAT3 targetPos;//目標座標
	float coreColChangeRaito = 0;//死亡時コアの色変え

	uint8_t partTimeInterval;
	uint8_t particleFrame;//パーティクル出すフレ

	//縦攻撃
	std::function<void()> verticalPattern;
	//一度離れてから突っ込む行動パターン　最初離れる
	std::function<void()> plungeIntoPattern;
	//突っ込み後の行動パターン 最初待ち
	std::function<void()> afterPlungePattern;

	std::function<void()> actionPattern;

	std::forward_list <std::unique_ptr<BossBullet>> bullets_;//ボスの弾　ユニークポインタ
	std::forward_list <std::unique_ptr<BossAimBul>> aimBullets_;//ボスの狙い弾
	std::forward_list <std::unique_ptr<BossStraightBul>> straightBullets_;//ボスの直線弾

	float time;

	//近づく速さ
	float approachSpZ;
	float approachSpY;
	//離れる速さ
	float leaveSpZ;
	float leaveSpY;

	//この時間だけこの行動をする
	uint16_t approachCountDef;
	uint16_t approachCount;

	bool bossEnemyAdvent = false;//ボス戦中 false:まだ

	//------縦に揺れる攻撃F
	int16_t changeVerticalCountDef;//デフォ0　終わったら消してん
	int16_t changeVerticalCount;
	int16_t changeVerticalNeces;//縦攻撃に移る為に必要カウント
	//最初の上昇値
	int16_t startVerticalValDef;
	int16_t startVerticalVal;
	//上昇下降速度
	float verticalSp;
	//待ち時間
	int16_t verticalWaitCountDef;
	int16_t verticalWaitCount;
	XMFLOAT3 upDownPosDef;	//上昇下降開始座標
	XMFLOAT3 upDownPos;
	float upStartPosY;//上昇開始Y座標
	float downStartPosY;//下降開始Y座標
	float nextMoveX;//UpDownの最後にXをずらす値
	//UpDown時の最初に開始位置決めるフラグ false:決める前
	bool verticalStartPosFlag = false;
	//次の動き false:これじゃない
	bool nextUp = false;
	bool nextDown = true;//最初は下降
	//UpDown何回ループさせるか
	int16_t verticalLoopCountDef;
	int16_t verticalLoopCount;
	//戻る行動開始座標に移動
	XMFLOAT3 reverseStartPos;
	//戻る行動開始座標を最初に一回やる　false:まだ
	bool reverseStartPosFlag = false;
	//縦攻撃用インターバル
	int16_t vertical_AtkInterval;

	//------

	//-------↓HPHALF↓------//
	bool isHpHalfPattern = false;//hp半分以下行動してない

	XMFLOAT3 hpHalfMomentPos = {};
	//まずこの位置に行く
	XMFLOAT3 targetHpHalfPos;
	float necesHpHalfFrame;//HP半分時このフレーム分移動まで時間かかる

	float necesLeaveFirstFrame;

	XMFLOAT4 coreCol;//コアの色　最初青
	//-------↑HPHALF↑------//

	//------HP半分以下円運動↓
	float hpHalf_AngleDef;
	float hpHalf_Angle;
	float hpHalf_radDef;
	float hpHalf_rad;

	float hpHalf_LengthDef;//円運動の半径の長さ
	float hpHalf_Length;

	float circularY;//どの高さで回るか

	XMFLOAT3 circlePosMem;//その時の座標取得
	//移動値
	const float addXDef = 0.f;
	float addX = addXDef;
	const float addYDef = 0.f;
	float addY = addYDef;
	//------HP半分以下円運動↑

	uint8_t circular_AtkIntervalDef;
	uint8_t circular_AtkInterval;

	BaseObject* shotTag;//弾うつターゲット

	//登場から行動を開始に移った時の座標
	XMFLOAT3 actionStartPos;

	//-------PlungeInto
	//2回Leaveをしたら突っ込む行動　　デフォ2
	uint32_t plungeCountDef;
	uint32_t plungeCount;
	//突っ込み行動へ移行する前に最後にいた場所を記憶する
	XMFLOAT3 wasPosMem;
	//離れる速度
	uint16_t leaveVel;
	//Leaveの時どの程度下がるか
	uint32_t leavePos;
	//戻る前の座標取得フラグ　false:してない
	bool beforeReversePosMemFlag = false;
	//戻る前座標入れる
	XMFLOAT3 beforeReversePosMem;
	//今の場所
	XMFLOAT3 reverseNowPos;
	//どこに戻るかの場所
	XMFLOAT3 reverseTargetPos;
	//突っ込み待ち待機時間 でふぉ140
	uint16_t plungeIntoWaitCountDef;
	uint16_t plungeIntoWaitCount;
	//最後定位置戻るのにかかる時間
	uint16_t plungeFinFrameMax;
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
	uint16_t plungeNecesFrame;
	//その時のボスの位置
	XMFLOAT3 plungeNowPos;
	//-------PlungeInto

	//-------AfterPlungeInto
	//
	//待ち時間
	int16_t waitTimeDef;
	int16_t waitTime;
	//攻撃時に自機のいた場所へ（XY）移動する速度
	XMFLOAT3 atkMoveSp;
	//ボスいた場所
	XMFLOAT3 boPosMem;
	//ボスの現在地
	XMFLOAT3 boNowPos;
	//攻撃移動にかかる絶対時間
	int16_t necesAtkMoveTime;
	//WaitとAttack繰り返した回数
	const int loopCountDef = 0;
	int loopCount = loopCountDef;
	//この回数でつぎへ
	int16_t loopCountMax;
	//突っ込み後行動のAimBulインターバル
	uint32_t afterPlungePatAtkInterval;
	//
	//-------AfterPlungeInto

	//<<<<<<シェイク
	int16_t randShakeVal;
	//いた場所保存 false:保存前
	bool shakePosMemFlag = false;
	//揺らし終えたか false:まだ
	bool isShakeFinished = false;
	//いた場所
	XMFLOAT3 posMem = {};
	//>>>>>>シェイク

	//<<<<<<ダメージ受けた時体赤くする演出
	int16_t bossBodyRedTimeDef;
	int16_t bossBodyRedTime;
	//赤から戻していく色
	float reCol;//赤状態
	float reColVal;//戻す数値
	//>>>>>>ダメージ受けた時体赤くする演出

	//敵HP
	int32_t bossMaxHP;//てきさいだいHP
	int32_t nowBossHP;//現在の敵HP

};