/// <<<<<<<<
/// @brief 自機のクラス
/// >>>>>>>>

#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "BaseObject.h"
#include "PlayerFireLine.h"
#include "ParticleManager.h"

#include <memory>
#include <list>//STL

class Player :public BaseObject
{
	DirectX::XMVECTOR lookVec{};

	DirectX::XMVECTOR position{};
private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Model* pBulModel = nullptr;
	Model* pFiringLine = nullptr;

	//攻撃のインターバルのためのフラグ 発射前
	bool attackIntervalFlag = false;

public:
	inline void SetPBulModel(Model* model) { pBulModel = model; }

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
	static const int32_t atkInterval = 15;
	//攻撃インターバルタイマー
	int32_t atkInterval_ = atkInterval;

	//揺れる時間
	static const int32_t pShakeTime = 60 / 2;
	//揺れたいまー
	int32_t pShakeTimer_ = pShakeTime;

	std::unique_ptr < PlayerFireLine> firingline_ = nullptr;//射線

	//-------------------↓げったーせったー↓-------------------//
	//弾威力
	void SetpBulPow(float pBulPower) { this->pBulPower = pBulPower; }
	const float& GetpBulPow() { return pBulPower; }
	//自機体力が0より少ないか
	void SetPHpLessThan0(bool isPHpLessThan0) { this->isPHpLessThan0 = isPHpLessThan0; }
	//自機体力が0より少ないか
	const bool& GetPHpLessThan0() { return isPHpLessThan0; }
	//自機安否確認
	const void SetpDeath(bool PlayerDeathFlag) { this->playerDeathFlag = PlayerDeathFlag; }
	const bool& GetpDeath() { return playerDeathFlag; }
	//射線表示フラグ
	const void SetFireLineDrawFlag(bool flag) { firingline_->SetAlive(flag); }
	//-------------------↑げったーせったー↑-------------------//

	//自機が攻撃できるようにするか true:攻撃できる
	bool pAtkPossibleFlag = false;

private:
	std::unique_ptr < Model> mod_firingline = nullptr;

	//パーティクル
	std::unique_ptr< ParticleManager> particle;

	//自機通常弾威力
	const float pBulPowerMax = 70;
	float pBulPower = pBulPowerMax;

	//自機体力が0より少ないとき false:　０より多い
	bool isPHpLessThan0 = false;

	const float pDeathRot = 0.4f;

	//-----自機死亡演出
	DirectX::XMFLOAT3 pPosDeath = {};
	float nowframe = 0;//現在フレ
	const float necesFrame = 180.0f;//かかる時間
	DirectX::XMFLOAT3 moveSp = {};//移動速度
	DirectX::XMFLOAT3 targetPos = { 0,-150,0 };//目標座標
	DirectX::XMFLOAT3 nowPos = {};//その時の位置

	float partTimeInterval = 0;
	float particleFrame = 39;//パーティクル出すフレ

	bool getPosFlag = true;//一度きりの座標読み込み

	bool playerDeathFlag = false;//自機死亡　false:ご存命

	//揺れ
	int randShakeDef = 0;
	int randShakeNow = randShakeDef;

	//自機死亡演出時　false:爆発してない
	bool explosionFlag = false;
};
