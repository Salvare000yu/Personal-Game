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
#include <unordered_map>

using namespace DirectX;

class GamePlayScene :public BaseScene, public BaseObject
{
	DirectX::XMFLOAT2 cameraMoveVel{};

	enum class BeforeBossPattern {
		def,
		inc,
		dec,
	};

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

	void GroundMove();

	void UpdateMouse();
	void UpdateCamera();

	//-----シーン移動
	void ChangeGameOverScene();//ゲームオーバーシーンへ

	void PadStickCamera();

	void PlayerErase();//自機死亡時消す

	void CollisionAll();//判定

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

	//void BossConditionComp();//ボス戦条件たっせい

	void DoorOpen();//扉が開く
	void pHeadingToTheNextPlace();//次の場所へ行く
	void CoolTime();
	void BeforeBossAppear();
	void BossDeathEffect();
	void BodyDamCoolTime();//体ダメージクールタイム

	void PlayerMove();
	void PlayerDash();//ダッシュ

	float time = 0;

	std::function<void()> updatePattern;

	//指定フレームごとに雑魚出現
	static const int SEneAppInterval = 60;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;
	std::unique_ptr < PlayerFireLine> firingline_ = nullptr;

	//揺れる時間
	static const int32_t pShakeTime = 60 / 2;
	//揺れたいまー
	int32_t pShakeTimer_ = pShakeTime;

	//自機ダメージフラグ 喰らってない
	bool pDamFlag = false;

	BeforeBossPattern beforeBossPattern_ = BeforeBossPattern::dec;

	//ダッシュする方向
	PlayerDashDirection playerDashDirection_ = PlayerDashDirection::def;

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_beforeboss = nullptr;
	std::unique_ptr < Sprite> sp_ready = nullptr;
	std::unique_ptr < Sprite> sp_ready_go = nullptr;

	std::unique_ptr < Model> mod_groundBottom = nullptr;//した地面
	std::unique_ptr < Object3d> obj_groundBottom = nullptr;
	std::unordered_map <std::string, std::unique_ptr < Model>> mod_ground;//地面
	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁
	std::unique_ptr < Model> mod_smallenemy = nullptr;//雑魚敵
	std::unique_ptr < Model> mod_playerbullet = nullptr;//自機弾
	std::unique_ptr < Model> mod_enemybullet = nullptr;//敵弾
	std::unique_ptr < Model> mod_bossaimbullet = nullptr;//敵狙い弾
	std::unique_ptr < Model> mod_straightbul = nullptr;//直線弾
	std::unique_ptr < Model> mod_player = nullptr;// 自機
	std::unique_ptr < Model> mod_bossColli = nullptr;
	std::unique_ptr < Model> mod_firingline = nullptr;
	std::unique_ptr < Model> mod_tunnel = nullptr;//トンネル
	std::unique_ptr < Model> mod_backwall = nullptr;//仮最後の壁

	std::unordered_map <std::string, std::unique_ptr < Object3d>> obj_ground;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;
	std::unique_ptr < Object3d> obj_tunnel = nullptr;
	std::unique_ptr < Object3d> obj_backwall = nullptr;

	// カメラ
	std::unique_ptr<CameraTracking> camera;

	//----ポストエフェクト関連
	//ヴィネット強さ
	float vignettePow = 0;

	//---地面
	//地面二種類を上下に揺らす
	const float SwingDist = 10.f;//揺らす距離
	const float SwingSp = 4.f;//揺らす速度
	float groundPosDef = -200;//地面を置く座標　ここ中心に上下

	//<<<<<開始時演出
	//bool gameReadyFlag = true;//開始演出中
	//ゲーム開始時フレーム
	int GameReadyFrame = 0;
	//開始時自機座標
	XMFLOAT3 ApStartPPos{};
	//終了
	XMFLOAT3 ApEndPPos{};

	//GOをだすフラグ
	bool ready_GOFlag = false;//false非表示
	//>>>>>開始時演出

	uint32_t frame = 0;

	bool pTracking = false;//true:自機追従中

	//雑魚敵出現用カウント
	float SEneAppCount = 0;

	//敵撃破数
	float sEnemyMurdersNum = 0;
	//ボス戦までの敵殺害必要数
	float BossTermsEMurdersNum = 1;
	//ボス出現条件達成！
	bool BossEnemyAdvent = false;
	//ボス出現前演出フラグ
	bool BeforeBossAppearFlag = false;
	//true:今やってる
	bool BeforeBossAppearNow = false;

	//ボスの体と衝突ダメージクールタイム false:喰らう前
	bool BodyDamFlag = false;
	//↑のクールタイムカウント
	const int BodyDamCountDef = 30;
	uint32_t BodyDamCount = BodyDamCountDef;

	int randShakeDef = 0;
	int randShakeNow = randShakeDef;

	int BBPaternCount = 0;//1→0で++

	bool AlertSoundFlag = true;//警告音繰り返しに一回のみ

	bool pRotDef = false;//カメラ最初にマウスの場所でズレちゃうから一度正面に向ける

	uint32_t pClearMoveCount = 120;//ボス撃破から自機移動開始まで
	uint32_t clearPMoveFrame = 0;//クリア演出自機移動ふれ
	XMFLOAT3 pBossBattlePos;
	uint32_t pRotReturnFrame = 0;
	XMFLOAT3 pClearRot;//ボス撃破時自機回転

	//ダメージ画面端赤く　false：まだやってない
	bool DamEfRedFlag = false;

	//自機動くなフラグ true:動 く な　false:動いてよい
	bool PDontMoveFlag = true;

	bool DoorOpenFlag = false;//扉開けてない

	//次の場所へ行くスピード
	const float pNextPlaceGoSpMax = 10.f;
	float pNextPlaceGoSp = 2.f;
	float AccelVal = 0.1f;//加速値
	float DecelVal = 0.2f;//減速値

	//与える威力
	float Damage{};

	std::vector<std::vector<std::string>> csvData;
	//何行目まで出したか
	int seIndex = -1;

	//----自機ダッシュ
	//false:してない
	bool DashFlag = false;
	//ダッシュ時間
	const uint32_t DashCountDef = 30;
	uint32_t DashCount = DashCountDef;
	//ダッシュカウントがこの分引いた値になったら減衰
	const int DashAttenuation = 10;
	bool DashAttenuationFlag = false;//減衰開始 fasle:まだしてない
	//減衰数値
	float Attenuation = -0.2f;
	//ダッシュクールインターバル
	const int DashIntervalDef = 40;
	int DashInterval = DashIntervalDef;
	bool DashIntervalFlag = false;//false:計測前 true:ダッシュできない時
	//ダッシュ速度
	XMFLOAT3 DashVel = { 0,0,0 };
	//実際に増やす値
	const float DashVelIncDef = 6;
	float DashVelInc = DashVelIncDef;
	
	//----自機ダッシュ
};
