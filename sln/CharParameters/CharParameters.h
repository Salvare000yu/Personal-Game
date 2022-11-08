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
	void BarGetDislodged();//��������
	void pHpUpdate();
	void boHpUpdate();
	void Draw()override;
	void pHpDraw();
	void boHpDraw();
	void DrawUI();

	//-----------------------���������[�������[��-----------------------//
	//���݃{�XHP
	void SetNowBoHp(float NowBossHP) { this->NowBossHP = NowBossHP; }
	const float& GetNowBoHp() { return NowBossHP; }
	//�{�X�h���
	void SetBossDefense(float BossDefense) { this->BossDefense = BossDefense; }
	const float& GetBossDefense() { return BossDefense; }
	//�v���C���[����HP
	void SetNowpHp(float NowPlayerHP) { this->NowPlayerHP = NowPlayerHP; }
	const float& GetNowpHp() { return NowPlayerHP; }
	//�v���C���[HPMax
	const float& GetpMaxHp() { return PlayerMaxHP; }
	//�{�XHPMax
	const float& GetboMaxHp() { return BossMaxHP; }
	//-----------------------���������[�������[��-----------------------//

private:
	std::unique_ptr < Sprite> sp_enemyhpbar = nullptr;
	std::unique_ptr < Sprite> sp_enemyhpbarwaku = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbar = nullptr;
	std::unique_ptr < Sprite> sp_playerhpbarwaku = nullptr;

	//�GHP
	const float BossMaxHP = 1000;//�Ă���������HP 
	float NowBossHP = BossMaxHP;//���݂̓GHP
	//�G�h���
	const float BossDefenseDef = 10;
	float BossDefense = BossDefenseDef;

	//���@HP
	const float PlayerMaxHP = 1000;//������������HP
	float NowPlayerHP = PlayerMaxHP;//���݂̎��@HP
	//�o�[�̂���]�]��������
	bool BarPosControlOnlyOnceFlag1 = false;
	bool BarPosControlOnlyOnceFlag2 = false;
	bool BarPosControlOnlyOnceFlag3 = false;
	bool BarPosControlOnlyOnceFlag4 = false;
	bool BarPosControlOnlyOnceFlag5 = false;
	bool BarPosControlOnlyOnceFlag6 = false;
	bool BarPosControlOnlyOnceFlag7 = false;
};

