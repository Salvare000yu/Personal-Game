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
#include "PlayerFireLine.h"

#include <memory>

using namespace DirectX;

class GamePlayScene :public BaseScene,public BaseObject
{

	DirectX::XMFLOAT2 cameraMoveVel{};

	enum class BeforeBossPattern {
		def,
		inc,
		dec,
	};

private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//// DirectX::を省略
	//using XMFLOAT2 = DirectX::XMFLOAT2;
	//using XMFLOAT3 = DirectX::XMFLOAT3;
	//using XMFLOAT4 = DirectX::XMFLOAT4;
	//using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize() override;

	void Finalize() override;

	void UpdateMouse();
	void UpdateCamera();

	void PadStickCamera();

	void CollisionAll();

	bool GameReady();

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void PlayTimer();

	void SmallEnemyAppear();

	void BeforeBossAppear();

	void BossDeathEfect();

	void PlayerMove();

	void CoolTime();

	XMVECTOR SplinePosition(const std::vector<XMVECTOR>& posints, size_t startIndex, float t);

	float time;

	//指定フレームごとに雑魚出現
	static const int SEneAppInterval = 80;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;
	std::list <std::unique_ptr<Boss>> boss_;

	//Enemy* enemy_ = nullptr;
	std::unique_ptr < Player> player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//雑魚敵リストを取得
	//const std::list<std::unique_ptr<SmallEnemy>>& GetSmallEnemys() { return smallEnemys_; }

	//揺れる時間
	static const int32_t pShakeTime = 60/2;
	//揺れたいまー
	int32_t pShakeTimer_ = pShakeTime;

	//自機ダメージフラグ 喰らってない
	bool pDamFlag = false;

	std::vector<XMVECTOR> points;
	size_t splineStartIndex;

	BeforeBossPattern beforeBossPattern_ = BeforeBossPattern::dec;

private:
	//sprite
	std::unique_ptr < Sprite> sprite_back = nullptr;
	//std::unique_ptr < Sprite> sp_guide = nullptr;
	std::unique_ptr < Sprite> sp_sight = nullptr;
	std::unique_ptr < Sprite> sp_beforeboss = nullptr;
	std::unique_ptr < Sprite> sp_ready = nullptr;
	std::unique_ptr < Sprite> sp_ready_go = nullptr;
	std::unique_ptr < Sprite> sp_blackwindow = nullptr;
	std::unique_ptr < Sprite> sp_dame_ef = nullptr;

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
	std::unique_ptr < Model> mod_firingline = nullptr;
	std::unique_ptr < Model> mod_tunnel = nullptr;//トンネル

	std::unique_ptr < Object3d> obj_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;
	std::unique_ptr < Object3d> obj_tunnel = nullptr;

	FbxModel* fbxModel_1 = nullptr;
	FbxObject3d* fbxObject_1 = nullptr;

	// カメラ
	std::unique_ptr<CameraTracking> camera;

	//GOをだすフラグ
	bool ready_GOFlag = false;//false非表示

	float frame = 0;

	//雑魚敵出現用カウント
	float SEneAppCount = 0;

	//敵通常弾威力
	const float eBulPower = 200;

	//敵撃破数
	float sEnemyMurdersNum = 0;
	//ボス戦までの敵殺害必要数
	float BossTermsEMurdersNum = 3;
	//ボス出現条件達成！
	bool BossEnemyAdvent = false;
	//ボス出現前演出フラグ
	bool BeforeBossAppearFlag = false;
	//true:今やってる
	bool BeforeBossAppearNow = false;
	
	int randShakeDef = 0;
	int randShakeNow=randShakeDef;

	int BBPaternCount = 0;//1→0で++

	bool AlertSoundFlag = true;//警告音繰り返しに一回のみ

	bool pRotDef = false;//カメラ最初にマウスの場所でズレちゃうから一度正面に向ける

	//黒画像を強くする値
	const float colordec = 0.006;

	//ダメージ画面端赤く　false：まだやってない
	bool DamEfRedFlag = false;

	//自機移動中かどうか false:してない
	bool isLMove = false;
	bool isRMove = false;
};

