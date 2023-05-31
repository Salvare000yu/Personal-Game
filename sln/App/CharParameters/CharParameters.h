#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Player.h"

class CharParameters :public BaseObject
{
public:
	static CharParameters* GetInstance();
	void Initialize() override;
	void pHpSizeChange();
	void boHpSizeChange();
	void Update()override;

	void PlayerHpSafety();
	void PlayerHpLessThanHalf();
	void PlayerHpDanger();

	void pHpUpdate();
	void BossHpSafety();
	void BossHpLessThanHalf();
	void BossHpDanger();
	void boHpUpdate();
	void Draw()override;
	void pHpDraw();
	void boHpDraw();
	void DrawUI();

	//-----------------------↓げったーせったー↓-----------------------//
	//現在ボスHP
	void SetNowBoHp(float nowBossHP) { this->nowBossHP = nowBossHP; }
	const float& GetNowBoHp() { return nowBossHP; }
	//自機現在HP
	void SetNowpHp(float nowPlayerHP) { this->nowPlayerHP = nowPlayerHP; }
	const float& GetNowpHp() { return nowPlayerHP; }
	//自機喰らった
	void SetispDam(bool ispDam) { this->ispDam = ispDam; }
	const bool& GetispDam() { return ispDam; }
	//自機HPMax
	const float& GetpMaxHp() { return playerMaxHP; }
	//ボスHPMax
	const float& GetBoMaxHp() { return bossMaxHP; }
	//ボス防御力
	void SetBossDefense(float bossDefense) { this->bossDefense = bossDefense; }
	const float& GetBossDefense() { return bossDefense; }
	//-----------------------↑げったーせったー↑-----------------------//

	const int stopPos = 1800;//自機次座標への移動の停止場所
	//次の場所へ向かう　true：向かっている false:完了
	bool pNextPlaceGoFlag = true;

	//自機HPバー色変えるパターン
	std::function<void()> pHpColorPattern;
	//bossHPバー色変えるパターン
	std::function<void()> boHpColorPattern;

private:
	std::unique_ptr < Sprite> sp_enemyhpbar = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbar = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbarwaku = nullptr;

	//敵HP
	const float bossMaxHP = 1000;//てきさいだいHP
	float nowBossHP = bossMaxHP;//現在の敵HP

	//自機HP
	const float playerMaxHP = 1000;//じきさいだいHP
	float nowPlayerHP = playerMaxHP;//現在の自機HP

	//------自機HPスプライト点滅
	const int pHpBarFrameDef = 20;//透明じゃない時間
	int pHpBarFrame = pHpBarFrameDef;
	//------ボスHPスプライト点滅
	const int boHpBarFrameDef = 20;//透明じゃない時間
	int boHpBarFrame = boHpBarFrameDef;

	//自機がダメージ喰らった false:喰らってない
	bool ispDam = false;

	//防御力
	const float bossDefenseDef = 50;
	float bossDefense = bossDefenseDef;
};
