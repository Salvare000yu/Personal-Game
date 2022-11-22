#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"

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
	//プレイヤー現在HP
	void SetNowpHp(float NowPlayerHP) { this->NowPlayerHP = NowPlayerHP; }
	const float& GetNowpHp() { return NowPlayerHP; }
	//プレイヤー喰らった
	void SetispDam(bool ispDam) { this->ispDam = ispDam; }
	const bool& GetispDam() { return ispDam; }
	//プレイヤーHPMax
	const float& GetpMaxHp() { return PlayerMaxHP; }
	//ボスHPMax
	const float& GetboMaxHp() { return BossMaxHP; }
	//-----------------------↑げったーせったー↑-----------------------//

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
};

