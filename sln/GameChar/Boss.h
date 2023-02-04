#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BossBullet.h"
#include "BossAimBul.h"
#include "BaseObject.h"

#include <memory>

class Boss:public BaseObject
{

	//行動パターン enumclass
	enum class ActionPattern {
		BossAppear,
		Approach,//近づいてくる
		Leave,//離れる
		HpHalfPatStart,//Hp半分以下になったらタゲまで移動
		CircularMotionMove,//ぐるぐる
		LeaveFirstPos,//元の場所へ一旦引く
		PlungeInto,//突っ込む
		Death,//死亡
	};

public:

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//攻撃
	void Attack();

	void PAimBul();
	void BossAppear();
	void Approach();
	void Leave();
	void HpHalfPatStart();
	void CircularMotionMove();
	void LeaveFirstPos();
	void PlungeInto();
	//拡散攻撃
	void DiffusionAttack();
	//拡散偶数弾
	void DiffusionAttackEavenNumber();

	void Death();

	//近づく処理初期化
	void ApproachInit();

	std::unique_ptr<Camera> camera; //カメラ
	
	float time;

	//近づく速さ
	const float ApproachSpZ = 1.f;
	const float ApproachSpY = 2.f;
	//離れる速さ
	const float LeaveSpZ = 1.f;
	const float LeaveSpY = 2.f;
	//近づける距離
	float ApproachLim;
	//離れられる距離
	float LeaveLim = 90;

	void  (Boss::* pFunc)();

	//初期パターン enumclassは　　　　型名　　:: 　列挙子
	ActionPattern actionPattern_ = ActionPattern::BossAppear;

	std::list <std::unique_ptr<BossBullet>> bullets_;//ボスの弾　ユニークポインタ
	std::list <std::unique_ptr<BossAimBul>> aimBullets_;//ボスの狙い弾

	//フレームごとに発射
	static const int AtkInterval = 10;
	static const int AtkInterval_LeaveFirst = 20;
	static const int DiffusionAtkInterval =20;

	//-----------------↓げったーせったー↓------------------//
	//弾リストを取得
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }
	const std::list<std::unique_ptr<BossAimBul>>& GetAimBullets() { return aimBullets_; }

	inline void SetBulModel(Model* model) { BulModel = model; }
	inline void SetAimBulModel(Model* model) { AimBulModel = model; }

	//通常弾威力
	const float& GetBulPow() { return BulPow; }
	//狙い弾威力
	const float& GetAimBulPow() { return AimBulPow; }

	//ボス生死
	void SetisDeath(float isDeath) { this->isDeath = isDeath; }
	const float& GetisDeath() { return isDeath; }

	//狙い弾　打つ相手
	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }
	//-----------------↑げったーせったー↑------------------//

private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//フレーム
	float frame = 0;
	//攻撃用カウント
	float AtkCount = 0;
	float DiffusionAtkCount = 0;
	float Circular_AtkCount = 0;	//ぐるぐる時攻撃用カウント

	bool even_odd_NumFlag = true;//奇数弾

	Model* BulModel = nullptr;
	Model* AimBulModel = nullptr;

	//敵通常弾威力
	const float BulPow = 200;
	const float AimBulPow = 150;

	//false：まだ死んでない
	bool isDeath = false;

	//死んだとき一度
	bool IsFirst_Death = false;

	XMFLOAT3 boPosDeath = {};
	bool GetPosFlag = true;//一度きりの座標読み込み
	bool GetPosDeathFlag = true;//一度きりの座標読み込み
	const float NecesFrame = 190.0f;//かかる時間
	float Nowframe = 0;//現在フレ
	XMFLOAT3 MoveSp = {};//移動速度
	XMFLOAT3 TargetPos = { 0,-150,0 };//目標座標
	XMFLOAT3 NowPos = {};//その時の位置

	float PartTimeInterval;
	float ParticleFrame = 39;//パーティクル出すフレ

	//------HP半分以下円運動↓
	const float HpHalf_AngleDef = 20;
	float HpHalf_Angle = HpHalf_AngleDef;
	const float HpHalf_radDef = 100;
	float HpHalf_rad = HpHalf_radDef;

	const float HpHalf_LengthDef = 10;//円運動の半径の長さ
	float HpHalf_Length = HpHalf_LengthDef;
	//移動値
	const float addXDef=0;
	float addX= addXDef;
	const float addYDef = 0;
	float addY = addYDef;
	//------HP半分以下円運動↑

	static const int Circular_AtkIntervalDef = 45;
	int Circular_AtkInterval = Circular_AtkIntervalDef;

	//-------↓HPHALF↓------//
	bool isHpHalfPattern = false;//hp半分以下行動してない

	XMFLOAT3 HpHalfMomentPos = {};
	//まずこの位置に行く
	XMFLOAT3 TargetHpHalfPos;
	const float NecesHpHalfFrame = 100.0f;//HP半分時このフレーム分移動まで時間かかる

	const float NecesLeaveFirstFrame = 180.f;
	//-------↑HPHALF↑------//

	//2回Leaveをしたら突っ込む行動
	const int PlungeCountDef = 2;
	int PlungeCount= PlungeCountDef;

	BaseObject* shotTag;//弾うつターゲット

	//登場から行動を開始に移った時の座標
	XMFLOAT3 ActionStartPos;

};