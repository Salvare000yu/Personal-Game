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
		HpHalf,//Hp半分以下になったら
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
	void HpHalf();
	//拡散攻撃
	void DiffusionAttack();
	//拡散偶数弾
	void DiffusionAttackEavenNumber();

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

	//弾リストを取得
	const std::list<std::unique_ptr<BossBullet>>& GetBullets() { return bullets_; }

	inline void SetEBulModel(Model* model) { eBulModel = model; }

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

};