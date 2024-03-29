﻿#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "BaseObject.h"
#include "CameraTracking.h"

#include "Camera.h"
#include "ParticleManager.h"

#include "FbxObject3d.h"

#include "Boss.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>
#include <unordered_map>

class GamePlayScene :public BaseScene
{
	DirectX::XMFLOAT2 cameraMoveVel{};

	enum class PlayerDashDirection {
		def,
		right,
		left,
		up,
		down,
	};

private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	void Initialize() override;

	void Finalize() override;

	void UpdateMouse();
	void UpdateCamera();

	//-----シーン移動
	void ChangeGameOverScene();//ゲームオーバーシーンへ

	void PadStickCamera();

	//自機Hpバー更新
	void PlayerHpUpdate();
	void PlayerHpSafety();//安全
	void PlayerHpLessThanHalf();//半分以下
	void PlayerHpDanger();//危険状態、赤色
	//ボスHp更新
	void BossHpUpdate();
	void BossHpSafety();
	void BossHpLessThanHalf();
	void BossHpDanger();

	void PlayerErase();//自機死亡時消す

	void PlayerDamage();//自機ダメージ
	void CollisionAll();//判定

	void Operation();//操作説明
	void MouseOper();//マウス説明

	void PauseOpen();//ポーズ画面開く

	//std::func
	void GameReadyUpdate();//開始前
	void SmallEnemyBattleUpdate();//雑魚戦
	void BossBattleReadyUpdate();//ボス戦開始前
	void BossBattleUpdate();//ボス戦
	void AfterBossBattleUpdate();//ボス撃破後

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void SmallEnemyCreate();//雑魚敵生成
	void SmallEnemyAppear();//雑魚敵出現
	void SmallEnemyAimBul();//雑魚的狙い弾

	void DoorOpen();//扉が開く
	void pHeadingToTheNextPlace();//次の場所へ行く
	void CoolTime();

	void BeforeBossAppear();
	void BeforeBossAppearDef();
	void BeforeBossAppearInc();
	void BeforeBossAppearDec();

	void BossDeathEffect();
	void BodyDamCoolTime();//体ダメージクールタイム

	void PlayerMove();
	void PlayerDash();//ダッシュ

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_beforeboss = nullptr;
	std::unique_ptr < Sprite> sp_ready = nullptr;
	std::unique_ptr < Sprite> sp_ready_go = nullptr;
	std::unordered_map <std::string, std::unique_ptr < Sprite>> sp_oper;//操作説明
	std::unordered_map <std::string, std::unique_ptr < Sprite>> sp_mouse;//マウス操作説明
	std::unique_ptr < Sprite> sp_playerhpbar = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbar = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbarwaku = nullptr;

	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁
	std::unique_ptr < Model> mod_smallenemy = nullptr;//雑魚敵
	std::unique_ptr < Model> mod_playerbullet = nullptr;//自機弾
	std::unique_ptr < Model> mod_enemybullet = nullptr;//敵弾
	std::unique_ptr < Model> mod_bossaimbullet = nullptr;//敵狙い弾
	std::unique_ptr < Model> mod_straightbul = nullptr;//直線弾
	std::unique_ptr < Model> mod_player = nullptr;// 自機
	std::unique_ptr < Model> mod_bossColli = nullptr;
	std::unique_ptr < Model> mod_backwall = nullptr;//仮最後の壁

	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;
	std::unique_ptr < Object3d> obj_backwall = nullptr;

	// カメラ
	std::unique_ptr<CameraTracking> camera;
	//パーティクル
	std::unique_ptr< ParticleManager> particle;

	//----ポストエフェクト関連
	//ヴィネット強さ
	float vignettePow = 0;

	//<<<<<開始時演出
	//ゲーム開始時フレーム
	uint16_t gameReadyFrame = 0;
	//開始時自機座標
	DirectX::XMFLOAT3 ApStartPPos{};
	//終了
	DirectX::XMFLOAT3 ApEndPPos{};

	//GOをだすフラグ
	bool ready_GOFlag = false;//false非表示
	//>>>>>開始時演出

	uint32_t frame = 0;

	bool pTracking = false;//true:自機追従中

	//雑魚敵出現用カウント
	int32_t sEneAppCount = 0;

	//敵撃破数
	uint32_t sEnemyMurdersNum = 0;
	//ボス戦までの敵殺害必要数
	uint32_t NeededBeforeBossBattleNum;
	//ボス出現前演出フラグ
	bool beforeBossAppearFlag = false;
	//true:今やってる
	bool beforeBossAppearNow = false;

	//ボスの体と衝突ダメージクールタイム false:喰らう前
	bool bodyDamFlag = false;
	//クールタイムカウント
	uint16_t bodyDamCountDef;
	uint16_t bodyDamCount;

	int randShakeDef = 0;
	int randShakeNow = randShakeDef;

	int bBPaternCount = 0;//1→0で++

	bool alertSoundFlag = true;//警告音繰り返しに一回のみ

	float WarningW;//透明度変化値

	bool pRotDef = false;//カメラ最初にマウスの場所でズレちゃうから一度正面に向ける

	uint32_t pClearMoveCount;//ボス撃破から自機移動開始まで
	uint32_t clearPMoveFrame = 0;//クリア演出自機移動ふれ
	uint32_t pRotReturnFrame = 0;
	uint32_t pRotReturnFrameMax;//この時間かけて戻す
	DirectX::XMFLOAT3 pClearRot;//ボス撃破時自機回転
	uint32_t clearMoveFrame;//クリア時この時間かけて移動する

	std::function<void()> playerHpBarPattern;//自機HPばー

	//ダメージ画面端赤く　false：まだやってない
	bool damEfRedFlag = false;

	//自機動くなフラグ true:動くな　false:動いてよい
	bool pDontMoveFlag = true;

	bool doorOpenFlag = false;//扉開けてない

	std::vector<std::vector<std::string>> SmallEnemyCsvData;
	//何行目まで出したか
	int32_t seIndex;

	//----自機ダッシュ
	//false:してない
	bool dashFlag = false;
	//ダッシュ時間
	uint32_t dashCountDef;
	uint32_t dashCount;
	//ダッシュカウントがこの分引いた値になったら減衰
	int16_t dashAttenuation;
	bool dashAttenuationFlag = false;//減衰開始 fasle:まだしてない
	//減衰数値
	float attenuation;
	//ダッシュクールインターバル
	uint16_t dashIntervalDef;
	uint16_t dashInterval;
	bool dashIntervalFlag = false;//false:計測前 true:ダッシュできない時
	//ダッシュ速度
	DirectX::XMFLOAT3 dashVel = { 0,0,0 };
	//実際に増やす値
	float dashVelIncDef;
	float dashVelInc;

	//----自機ダッシュ

	float time = 0;

	std::function<void()> updatePattern;

	//指定フレームごとに雑魚出現
	int16_t sEneAppInterval;

	std::forward_list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::forward_list <std::unique_ptr<Boss>> boss_;

	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//揺れる時間
	static const int32_t pShakeTime = 60 / 2;
	//揺れたいまー
	int32_t pShakeTimer_ = pShakeTime;

	//自機ダメージフラグ 喰らってない
	bool pDamFlag = false;

	//ボス戦前演出
	std::function<void()> beforeBossPattern;

	//ダッシュする方向
	PlayerDashDirection playerDashDirection_ = PlayerDashDirection::def;

	//------自機HPスプライト点滅
	int32_t pHpBarFrameDef;//透明じゃない時間
	int32_t pHpBarFrame;

	bool pHpBarFlag = false;//自機Hpバー false:非表示

	//bossHPバー色変えるパターン
	std::function<void()> boHpColorPattern;
	//------ボスHPスプライト点滅
	int32_t boHpBarFrameDef;//透明じゃない時間
	int32_t boHpBarFrame;
};
