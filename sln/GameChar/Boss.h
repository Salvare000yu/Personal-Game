#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BossBullet.h"
#include "BaseObject.h"

#include <memory>

class Boss:public BaseObject
{

	//行動パターン enumclass
	enum class ActionPattern {
		Approach,//近づいてくる
		Leave,//離れる
		HpHalfPatStart,//Hp半分以下になったらタゲまで移動
		HpHalf,
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
	void Approach();
	void Leave();
	void HpHalfPatStart();
	void HpHalf();
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
	const float ApproachSpZ = 1;
	const float ApproachSpY = 2;
	//離れる速さ
	const float LeaveSpZ = 1;
	const float LeaveSpY = 2;
	//近づける距離
	const float ApproachLim = -50;
	//離れられる距離
	const float LeaveLim = 90;

	void  (Boss::* pFunc)();

	//初期パターン enumclassは　　　　型名　　:: 　列挙子
	ActionPattern actionPattern_ = ActionPattern::Approach;

	std::list <std::unique_ptr<BossBullet>> bullets_;//ボスの弾　ユニークポインタ

	//フレームごとに発射
	static const int AtkInterval = 10;
	static const int DiffusionAtkInterval =20;

	//-----------------↓げったーせったー↓------------------//
	//弾リストを取得
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }

	inline void SetEBulModel(Model* model) { eBulModel = model; }

	//ボス防御力
	void SetBossDefense(float BossDefense) { this->BossDefense = BossDefense; }
	const float& GetBossDefense() { return BossDefense; }

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

	bool even_odd_NumFlag = true;//奇数弾

	Model* eBulModel = nullptr;

	//防御力
	const float BossDefenseDef = 10;
	float BossDefense = BossDefenseDef;

	//false：まだ死んでない
	bool isDeath = false;

	XMFLOAT3 boPosDeath = {};
	bool GetPosFlag = true;//一度きりの座標読み込み
	const float NecesFrame = 190.0f;//かかる時間
	float Nowframe = 0;//現在フレ
	XMFLOAT3 MoveSp = {};//移動速度
	XMFLOAT3 TargetPos = { 0,-150,0 };//目標座標
	XMFLOAT3 NowPos = {};//その時の位置

	float PartTimeInterval;
	float ParticleFrame = 39;//パーティクル出すフレ

	//-------↓HPHALF↓------//
	bool isHpHalfPattern = false;//hp半分以下行動してない

	XMFLOAT3 HpHalfMomentPos = {};
	//まずこの位置に行く
	XMFLOAT3 TargetHpHalfPos = { 0, 40, 200 };
	const float NecesHpHalfFrame = 180.0f;//HP半分時このフレーム分移動まで時間かかる
	//-------↑HPHALF↑------//

	BaseObject* shotTag;//弾うつターゲット
};