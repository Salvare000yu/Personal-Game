﻿//直線弾
#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class BossStraightBul :public BaseObject
{
private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	static BossStraightBul* GetInstance();

	std::unique_ptr<Camera> camera; //カメラ

	float time;
	float frame = 0;

	XMFLOAT3 velocity{ 0,0,0 };

	//消えるまで
	static const int32_t bulVanishTime = 60 * 7;//寿命

	//消滅の宣告
	int32_t vanishTimer_ = bulVanishTime;

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }

	//直線弾スピード
	float straightBulSp = 0.07f;
	//直線弾加速
	float straightBulAccel = 0.1f;
	//速度制限
	const int straightBulSpLim = 30;
};
