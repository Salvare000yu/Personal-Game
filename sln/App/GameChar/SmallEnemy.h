#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"
#include "SmallEnemyBullet.h"

#include <memory>

class SmallEnemy :public BaseObject
{
	//捌けパターン
	enum class RetirePat {
		Right,
		Left,
		def,
	};

private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Attack();

	//初期化
	void Initialize()override;

	void Retire();

	void BulletUpdate();

	//更新
	void Update()override;

	//描画
	void Draw()override;

	std::unique_ptr<Camera> camera; //カメラ

	//左右に捌ける
	static const int32_t retireFrameDef = 120;
	int32_t retireFrame = retireFrameDef;

	std::list <std::unique_ptr<SmallEnemyBullet>> bullets_;//プレイヤーの弾　ユニークポインタ

	//-----------------↓げったーせったー↓------------------//
	//弾リストを取得
	const std::list<std::unique_ptr<SmallEnemyBullet>>& GetBullets() { return bullets_; }

	inline void SetSEBulModel(Model* model) { seBulModel = model; }

	//通常弾威力
	void SetBulPow(float seBulPower) { this->seBulPower = seBulPower; }
	const float& GetBulPow() { return seBulPower; }

	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }
	//-----------------↑げったーせったー↑------------------//

	//フレームごとに発射
	static const int atkInterval = 90;

	RetirePat retirePat_ = RetirePat::def;

private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//SEneXの乱数入れる変数
	float sEneRandX = 0;

	//攻撃用カウント
	float atkCount = 0;
	Model* seBulModel = nullptr;

	//通常弾威力
	const float seBulPowerMax = 100;
	float seBulPower = seBulPowerMax;

	BaseObject* shotTag;//弾うつターゲット

	//ここまで来たら止まって捌ける
	const int posZMax = 420;
	//捌け開始
	bool isRetire = false;
	//向かってくる間だけ
	bool isSeApproach = true;
};