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
	void BarGetDislodged();//いつか消す
	void pHpUpdate();
	void boHpUpdate();
	void Draw()override;
	void pHpDraw();
	void boHpDraw();
	void DrawUI();

	//-----------------------↓げったーせったー↓-----------------------//
	//現在ボスHP
	void SetNowBoHp(float NowBossHP) { this->NowBossHP = NowBossHP; }
	const float& GetNowBoHp() { return NowBossHP; }
	//自機現在HP
	void SetNowpHp(float NowPlayerHP) { this->NowPlayerHP = NowPlayerHP; }
	const float& GetNowpHp() { return NowPlayerHP; }
	//自機喰らった
	void SetispDam(bool ispDam) { this->ispDam = ispDam; }
	const bool& GetispDam() { return ispDam; }
	//自機HPMax
	const float& GetpMaxHp() { return PlayerMaxHP; }
	//ボスHPMax
	const float& GetboMaxHp() { return BossMaxHP; }
	//ボス防御力
	void SetBossDefense(float BossDefense) { this->BossDefense = BossDefense; }
	const float& GetBossDefense() { return BossDefense; }
	//-----------------------↑げったーせったー↑-----------------------//

	const int StopPos = 1800;//自機次座標への移動の停止場所
	//次の場所へ向かう　true：向かっている false:完了
	bool pNextPlaceGoFlag = true;

private:
	std::unique_ptr < Sprite> sp_enemyhpbar = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbar = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbarwaku = nullptr;

	//敵HP
	const float BossMaxHP = 1000;//てきさいだいHP 
	float NowBossHP = BossMaxHP;//現在の敵HP

	//自機HP
	const float PlayerMaxHP = 1000;//じきさいだいHP
	float NowPlayerHP = PlayerMaxHP;//現在の自機HP

	//自機がダメージ喰らった false:喰らってない
	bool ispDam = false;

	//バーのずれ‐‐いつか消す
	bool BarPosControlOnlyOnceFlag1 = false;
	bool BarPosControlOnlyOnceFlag2 = false;
	bool BarPosControlOnlyOnceFlag3 = false;
	bool BarPosControlOnlyOnceFlag4 = false;
	bool BarPosControlOnlyOnceFlag5 = false;
	bool BarPosControlOnlyOnceFlag6 = false;
	bool BarPosControlOnlyOnceFlag7 = false;

	//防御力
	const float BossDefenseDef = 60;
	float BossDefense = BossDefenseDef;

};

