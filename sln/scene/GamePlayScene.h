#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseObject.h"
#include "CameraTracking.h"

#include "Camera.h"

#include "FbxObject3d.h"

#include "Boss.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>

class GamePlayScene :public BaseScene,public BaseObject
{

	DirectX::XMFLOAT2 cameraMoveVel{};
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize() override;

	void Finalize() override;

	void UpdateMouse();
	void UpdateCamera();

	void CollisionAll();

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void SmallEnemyAppear();

	void PlayerDeath();

	void PlayerMove();

	void CoolTime();

	float time;

	//指定フレームごとに雑魚出現
	static const int SEneAppInterval = 80;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//敵
	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//雑魚敵リストを取得
	//const std::list<std::unique_ptr<SmallEnemy>>& GetSmallEnemys() { return smallEnemys_; }

	//揺れる時間
	static const int32_t pShakeTime = 60/2;
	//揺れたいまー
	int32_t pShakeTimer_ = pShakeTime;

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_guide = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbar = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbar = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_semeter = nullptr;

	std::unique_ptr < Model> mod_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Model> model_1 = nullptr;//地面
	std::unique_ptr < Model> mod_worlddome = nullptr;//天球
	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁
	std::unique_ptr < Model> mod_smallenemy = nullptr;//雑魚敵
	std::unique_ptr < Model> mod_playerbullet = nullptr;//自機弾
	std::unique_ptr < Model> mod_enemybullet = nullptr;//敵弾
	std::unique_ptr < Model> mod_player = nullptr;// 自機
	std::unique_ptr < Model> mod_enemy = nullptr;

	std::unique_ptr < Object3d> obj_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1 = nullptr;

	// カメラ
	std::unique_ptr<CameraTracking> camera;

	float frame = 0;

	//雑魚敵出現用カウント
	float SEneAppCount = 0;

	//自機通常弾威力
	const float pBulPower = 50;
	//敵通常弾威力
	const float eBulPower = 200;
	//雑魚敵通常弾威力
	const float seBulPower = 100;

	//敵HP
	const float BossMaxHP = 1000;//てきさいだいHP 
	float NowBossHP = BossMaxHP;//現在の敵HP

	//自機HP
	const float PlayerMaxHP = 1000;//じきさいだいHP
	float NowPlayerHP = PlayerMaxHP;//現在の自機HP
	//バーのずれ‐‐いつか消す
	bool BarPosControlOnlyOnceFlag1 = false;
	bool BarPosControlOnlyOnceFlag2 = false;
	bool BarPosControlOnlyOnceFlag3 = false;
	bool BarPosControlOnlyOnceFlag4 = false;
	bool BarPosControlOnlyOnceFlag5 = false;
	bool BarPosControlOnlyOnceFlag6 = false;
	bool BarPosControlOnlyOnceFlag7 = false;
	//自機ダメージフラグ 喰らってない
	bool pDamFlag = false;
	//敵撃破数
	float sEnemyMurdersNum = 0;
	//ボス戦までの敵殺害必要数
	float BossTermsEMurdersNum = 5;
	//ボス出現条件達成！
	bool BossEnemyAdvent = false;
	
	int randShakeDef = 0;
	int randShakeNow=randShakeDef;
};

