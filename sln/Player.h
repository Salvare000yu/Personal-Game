#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "PlayerBullet.h"
#include "BaseObject.h"

#include <memory>
#include <list>//STL

class Player:public BaseObject
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

	float frame = 0;
public:
	//初期化
	void Initialize() override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	//攻撃
	void Attack();

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	//PlayerBullet* bullet_ = nullptr;
	std::list <std::unique_ptr<PlayerBullet>> bullets_;//プレイヤーの弾　ユニークポインタ

	//モデル
	std::unique_ptr < Model> mod_player = nullptr;

	//当たった時呼び出すためのコールバック関数
	void OnCollision();
	//弾リストを取得 gamesceneに自弾貸すため
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//弾座標判定で使うためのgetbulletposmemory
	static XMFLOAT3 Player::GetPlayerPosMemory();
	//静的メンバ変数　当たり判定で使う為の弾座標
	static XMFLOAT3 PlayerPosMemory;

};

