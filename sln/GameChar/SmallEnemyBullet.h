#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class SmallEnemyBullet :public BaseObject
{
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

	//float frame = 0;

	XMFLOAT3 velocity{ 0,0,0 };

public:
	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	static SmallEnemyBullet* GetInstance();

	std::unique_ptr<Camera> camera; //カメラ

	//float time;

	//消えるまで
	static const int32_t BulVanishTime = 60 * 5;//寿命

	//消滅の宣告
	int32_t vanishTimer_ = BulVanishTime;

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }

	float Nowframe = 0;//現在フレ
	bool GetPosOnlyFlag = true;//一度きりの座標読み込み
	XMFLOAT3 NowPos;//その時の弾位置
	XMFLOAT3 sePosMoment;//発射時の雑魚敵位置
	XMFLOAT3 MoveSp;//弾移動速度

	bool ShotTagMomOnlyFlag = true;//一度きりのターゲット座標読み込み
	XMFLOAT3 ShotTagMoment{};

private:

	float tmp;
	short angle;
};
