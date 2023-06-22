#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class BossAimBul :public BaseObject
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	float frame = 0;

	XMFLOAT3 velocity{ 0,0,0 };

public:
	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	static BossAimBul* GetInstance();

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	//消えるまで
	static const int32_t bulVanishTime = 60 * 10;//寿命

	//消滅の宣告
	int32_t vanishTimer_ = bulVanishTime;

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }

	bool shotTagMomOnlyFlag = true;//一度きりのターゲット座標読み込み
	XMFLOAT3 shotTagMoment{};
	float nowframe = 0;//現在フレ
	bool getPosOnlyFlag = true;//一度きりの座標読み込み
	XMFLOAT3 boPosMoment;//発射時の雑魚敵位置
	XMFLOAT3 moveSp;//弾移動速度
	XMFLOAT3 nowPos;//その時の弾位置
};
