#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "EnemyBullet.h"
#include "BaseObject.h"

#include <memory>

class Enemy:public BaseObject
{

	//行動パターン enumclass
	enum class ActionPattern {
		Approach,//近づいてくる
		Leave,//離れる
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

	//近づく処理初期化
	void ApproachInit();

	std::unique_ptr<Camera> camera; //カメラ
	
	float time;

	//近づく速さ
	const float ApproachSp = 1;
	//近づける距離
	const float ApproachLim = -50;
	//離れられる距離
	const float LeaveLim = 90;

	//初期パターン enumclassは　　　　型名　　:: 　列挙子
	ActionPattern actionPattern_ = ActionPattern::Approach;

	std::list <std::unique_ptr<EnemyBullet>> bullets_;//プレイヤーの弾　ユニークポインタ

	//フレームごとに発射
	static const int AtkInterval = 30;

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	inline void SetEBulModel(Model* model) { eBulModel = model; }

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

	//フレーム
	float frame = 0;
	//攻撃用カウント
	float AtkCount = 0;

	Model* eBulModel = nullptr;
};