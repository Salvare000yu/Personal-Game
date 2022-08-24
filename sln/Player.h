#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"

#include <memory>
#include <list>//STL

class Player
{
public:
	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//攻撃
	void Attack();

	static Player* GetInstance();

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	float PlayerposX = 0;
	float PlayerposY = 0;
	float PlayerposZ = 0;

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//プレイヤーの弾　ユニークポインタ

	//モデル
	std::unique_ptr < Model> mod_player = nullptr;
	//オブジェ3d
	std::unique_ptr < Object3d> obj_player = nullptr;

	//当たった時呼び出すためのコールバック関数
	void OnCollision();
	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

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

	float frame = 0;
};

