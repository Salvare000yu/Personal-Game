#include "CharParameters.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "DebugText.h"
#include <DirectXMath.h>

using namespace DirectX;

CharParameters* CharParameters::GetInstance()
{
	static CharParameters instance;
	return &instance;
}
void CharParameters::Initialize()
{
	// -----------------�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/HPbar.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/HPbar_waku.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/playerHPbar.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/playerHPbar_waku.png");

	//�X�v���C�g����
	sp_enemyhpbar.reset(Sprite::Create(3, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbarwaku.reset(Sprite::Create(4, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbar.reset(Sprite::Create(5, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbarwaku.reset(Sprite::Create(6, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//�X�v���C�g�|�W�V����
	sp_enemyhpbar->SetPosition({ 140,-80,0 });
	sp_enemyhpbarwaku->SetPosition({ 140,-80,0 });
	sp_playerhpbar->SetPosition({ -70,500,0 });
	sp_playerhpbarwaku->SetPosition({ -70,500,0 });
	//�X�v���C�g�T�C�Y
	//sp_playerhpbarwaku->size_.x = PlayerMaxHP;
	//sp_playerhpbarwaku->TransferVertexBuffer();
	BarPosControlOnlyOnceFlag1 = false;
	BarPosControlOnlyOnceFlag2 = false;
	BarPosControlOnlyOnceFlag3 = false;
	BarPosControlOnlyOnceFlag4 = false;
	BarPosControlOnlyOnceFlag5 = false;
	BarPosControlOnlyOnceFlag6 = false;
	BarPosControlOnlyOnceFlag7 = false;

	//�p�����[�^�֘A������
	BossDefense = BossDefenseDef;
	NowBossHP = BossMaxHP;//
	NowPlayerHP = PlayerMaxHP;//

}

void CharParameters::pHpSizeChange()
{
	sp_playerhpbar->size_.x = sp_playerhpbar->texSize_.x * (float)NowPlayerHP / PlayerMaxHP;
	sp_playerhpbar->TransferVertexBuffer();
}
void CharParameters::boHpSizeChange()
{
	//�T�C�Y�ύX
	sp_enemyhpbar->size_.x = sp_enemyhpbar->texSize_.x * (float)NowBossHP / BossMaxHP;
	sp_enemyhpbar->TransferVertexBuffer();
}

void CharParameters::Update()
{

}

void CharParameters::BarGetDislodged()
{
	//�T�C�Y�ύX�ɂ��Y��--�����������牼
	{
		XMFLOAT3 pHpBar = sp_playerhpbar->GetPosition();
		if (NowPlayerHP <= 700 && BarPosControlOnlyOnceFlag3 == false) {
			pHpBar.x += 30;
			BarPosControlOnlyOnceFlag3 = true;
		}
		if (NowPlayerHP <= 600 && BarPosControlOnlyOnceFlag1 == false) {
			pHpBar.x += 10;
			BarPosControlOnlyOnceFlag1 = true;
		}
		if (NowPlayerHP <= 500 && BarPosControlOnlyOnceFlag4 == false) {
			pHpBar.x += 10;
			BarPosControlOnlyOnceFlag4 = true;
		}
		if (NowPlayerHP <= 400 && BarPosControlOnlyOnceFlag5 == false) {
			pHpBar.x += 10;
			BarPosControlOnlyOnceFlag5 = true;
		}
		if (NowPlayerHP <= 300 && BarPosControlOnlyOnceFlag6 == false) {
			pHpBar.x += 10;
			BarPosControlOnlyOnceFlag6 = true;
		}
		if (NowPlayerHP <= 200 && BarPosControlOnlyOnceFlag7 == false) {
			pHpBar.x += 10;
			BarPosControlOnlyOnceFlag7 = true;
		}
		sp_playerhpbar->SetPosition(pHpBar);
	}
}

void CharParameters::pHpUpdate()
{
	sp_playerhpbar->Update();
	sp_playerhpbarwaku->Update();
}

void CharParameters::boHpUpdate()
{
	//�X�V
	sp_enemyhpbar->Update();
	sp_enemyhpbarwaku->Update();
}

void CharParameters::Draw()
{
}
void CharParameters::pHpDraw()
{
	sp_playerhpbar->Draw();
	sp_playerhpbarwaku->Draw();
}
void CharParameters::boHpDraw()
{
	sp_enemyhpbar->Draw();
	sp_enemyhpbarwaku->Draw();
}

void CharParameters::DrawUI()
{
	////���@�ƓG�̗̑�
	//{
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%2.f", NowPlayerHP);
	//	DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	//}
	//{
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%2.f", NowBossHP);
	//	DebugText::GetInstance()->Print(tmp, 430, 460, 3);
	//}
}