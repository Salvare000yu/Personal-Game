#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "BaseObject.h"
#include "PlayerFireLine.h"

#include <memory>
#include <list>//STL

class Player :public BaseObject
{

	DirectX::XMVECTOR lookVec{};

	DirectX::XMVECTOR position{};
private:
	////-----------------model
	//std::unique_ptr < Model> mod_classplayer = nullptr;//自機

	////-----------------obj
	//std::unique_ptr < Object3d> obj_classplayer = nullptr;//自機

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	Model* pBulModel = nullptr;
	Model* pFiringLine = nullptr;
	std::unique_ptr < PlayerFireLine> firingline_ = nullptr;

	//攻撃のインターバルのためのフラグ 発射前
	bool AttackIntervalFlag = false;

public:
	inline void SetPBulModel(Model* model) { pBulModel=model; }
	inline void SetPFiringLine(Model* model) { pFiringLine = model; }

	/*static Player* GetInstance();*/

	//初期化
	void Initialize() override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//攻撃
	void Attack();

	void Move();

	void Shake();

	void PlayerDeath();

	void FiringLine();

	std::unique_ptr<Camera> camera; //カメラ

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//プレイヤーの弾　ユニークポインタ

	//弾リストを取得 gamesceneに自弾貸すため
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	Player()
		: lookVec(DirectX::XMVectorSet(0, 0, 1, 0)),
		position(DirectX::XMVectorSet(0, 0, 0, 1)) {
	}
	//向き取得
	inline const DirectX::XMVECTOR& GetLookVec() { return lookVec; }
	inline DirectX::XMVECTOR GetPosVec() { return position; }

	//攻撃インターバル
	static const int32_t AtkInterval = 10;
	//攻撃インターバルタイマー
	int32_t AtkInterval_ = AtkInterval;

	//揺れる時間
	static const int32_t pShakeTime = 60 / 2;
	//揺れたいまー
	int32_t pShakeTimer_ = pShakeTime;

	//-------------------↓げったーせったー↓-------------------//
	//弾威力
	void SetpBulPow(float pBulPower) { this->pBulPower = pBulPower; }
	const float& GetpBulPow() { return pBulPower; }
	//プレイ中かどうか
	void SetReadyNow(bool ReadyNowFlag) { this->ReadyNowFlag = ReadyNowFlag; }
	const bool& GetReadyNow() { return ReadyNowFlag; }
	//自機体力が0より少ないか多いか
	void SetPHpLessThan0(bool isPHpLessThan0) { this->isPHpLessThan0 = isPHpLessThan0; }
	const bool& GetPHpLessThan0() { return isPHpLessThan0; }
	//自機安否確認
	void SetpDeath(bool PlayerDeathFlag) { this->PlayerDeathFlag = PlayerDeathFlag; }
	const bool& GetpDeath() { return PlayerDeathFlag; }
	//-------------------↑げったーせったー↑-------------------//

private:
	//自機通常弾威力
	const float pBulPowerMax = 200;
	float pBulPower = pBulPowerMax;

	//playのReady中かどうか
	bool ReadyNowFlag = true;//true：やってる
	//自機体力が0より少ないとき false:　０より多い
	bool isPHpLessThan0 = false;

	const float pDeathRot = 0.4f;

	//-----自機死亡演出
	XMFLOAT3 pPosDeath = {};
	float Nowframe = 0;//現在フレ
	const float NecesFrame = 180.0f;//かかる時間
	XMFLOAT3 MoveSp = {};//移動速度
	XMFLOAT3 TargetPos = { 0,-150,0 };//目標座標
	XMFLOAT3 NowPos = {};//その時の位置

	float PartTimeInterval;
	float ParticleFrame = 39;//パーティクル出すフレ

	bool GetPosFlag = true;//一度きりの座標読み込み

	bool PlayerDeathFlag = false;//自機死亡　false:ご存命

	//揺れ
	int randShakeDef = 0;
	int randShakeNow = randShakeDef;

	//自機死亡演出時　false:爆発してない
	bool ExplosionFlag = false;
};

