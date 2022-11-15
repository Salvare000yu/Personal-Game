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

	//初期化
	void Initialize() override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//攻撃
	void Attack();

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

	//-------------------↓げったーせったー↓-------------------//
	//弾威力
	void SetpBulPow(float pBulPower) { this->pBulPower = pBulPower; }
	const float& GetpBulPow() { return pBulPower; }
	//プレイ中かどうか
	void SetReadyNow(bool ReadyNowFlag) { this->ReadyNowFlag = ReadyNowFlag; }
	const bool& GetReadyNow() { return ReadyNowFlag; }
	//-------------------↑げったーせったー↑-------------------//

private:
	//自機通常弾威力
	const float pBulPowerMax = 50;
	float pBulPower = pBulPowerMax;

	//playのReady中かどうか
	bool ReadyNowFlag = true;//true：やってる
};

