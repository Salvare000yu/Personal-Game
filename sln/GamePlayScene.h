#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseObject.h"


#include "Camera.h"

#include "FbxObject3d.h"

#include "Boss.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>

class GamePlayScene :public BaseScene,public BaseObject
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void SmallEnemyAppear();

	void Obj2move();

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	//指定フレームごとに雑魚出現
	static const int SEneAppInterval = 50;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//敵
	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//雑魚敵リストを取得
	//const std::list<std::unique_ptr<SmallEnemy>>& GetSmallEnemys() { return smallEnemys_; }

	//揺れる時間
	static const int32_t pShakeTime = 60*3;
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

	float frame = 0;

	//雑魚敵出現用カウント
	float SEneAppCount = 0;

	//自機通常弾威力
	const float pBulPower = 50;
	//敵通常弾威力
	const float eBulPower = 200;

	//敵HP
	const int BossMaxHP = 1000;//てきさいだいHP 
	int NowBossHP = BossMaxHP;//現在の敵HP

	//自機HP
	const int PlayerMaxHP = 1000;//じきさいだいHP
	int NowPlayerHP = PlayerMaxHP;//現在の自機HP
	bool BarPosControlOnlyOnceFlag1 = false;
	bool BarPosControlOnlyOnceFlag2 = false;
	//自機ダメージフラグ 喰らってない
	bool pDamFlag = false;
	//敵撃破数
	int sEnemyMurdersNum = 0;
	//ボス戦までの敵殺害必要数
	int BossTermsEMurdersNum = 8;
	//ボス出現条件達成！
	bool BossEnemyAdvent = false;
	
};

