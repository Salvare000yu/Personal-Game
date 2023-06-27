#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class SmallEnemyBullet :public BaseObject
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

	static SmallEnemyBullet* GetInstance();

	std::unique_ptr<Camera> camera; //カメラ

	//float time;

	//消えるまで
	static const int32_t BulVanishTime = 60 * 5;//寿命

	//消滅の宣告
	int32_t vanishTimer_ = BulVanishTime;

	XMFLOAT3 velocity{ 0,0,0 };

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }

	float nowFrame = 0;//現在フレ
	bool onlyFlag = true;//一度
	XMFLOAT3 nowPos;//その時の弾位置
	XMFLOAT3 sePosMoment;//発射時の雑魚敵位置
	XMFLOAT3 moveSp;//弾移動速度

	XMFLOAT3 shotTagMoment{};

private:

	float tmp;
	short angle;
};
