//きゃら固有値
#include "CharParameters.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "DebugText.h"
#include <DirectXMath.h>

#include <algorithm>

using namespace DirectX;

CharParameters* CharParameters::GetInstance()
{
	static CharParameters instance;
	return &instance;
}
void CharParameters::Initialize()
{
	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/HPbar.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/HPbar_waku.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/playerHPbar.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/playerHPbar_waku.png");

	//スプライト生成
	sp_enemyhpbar.reset(Sprite::Create(3, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbarwaku.reset(Sprite::Create(4, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//スプライトポジション
	sp_enemyhpbar->SetPosition({ 140,-80,0 });
	sp_enemyhpbarwaku->SetPosition({ 90,-80,0 });
	//ｰｰ色
	//ボスHPバー最初の色
	sp_enemyhpbar->SetColor({ 0.4f,1,0.4f,1 });

	//パラメータ関連初期化
	bossDefense = bossDefenseDef;
	nowBossHP = bossMaxHP;//
	pNextPlaceGoFlag = true;

	//HP色を設定 最初緑
	boHpColorPattern = std::bind(&CharParameters::BossHpSafety, this);
}

void CharParameters::boHpSizeChange()
{
	//サイズ変更
	sp_enemyhpbar->size_.x = sp_enemyhpbar->texSize_.x * (float)nowBossHP / bossMaxHP;
	sp_enemyhpbar->TransferVertexBuffer();
}

void CharParameters::Update()
{
}

void CharParameters::BossHpSafety()
{
	//半分以下で
	if (nowBossHP <= bossMaxHP / 2.f) {
		///自機HPバー半分以下黄色
		sp_enemyhpbar->SetColor({ 1,1,0,1 });
		boHpColorPattern = std::bind(&CharParameters::BossHpLessThanHalf, this);
	}
}
void CharParameters::BossHpLessThanHalf()
{
	//HP指定した値まで減ったら
	if (nowBossHP <= bossMaxHP / 4.f) {
		sp_enemyhpbar->SetColor({ 1,0,0,1 });//赤
		boHpColorPattern = std::bind(&CharParameters::BossHpDanger, this);
	}
}
void CharParameters::BossHpDanger()
{
	constexpr float ColorWDec = 0.015f;//透明にしていく速度
	constexpr float Transparency = 0.4f;//最終的な透明度がどこまで行くか。ここまでいったらデフォ値に戻す

	XMFLOAT4 color = sp_enemyhpbar->GetColor();

	//---自機と同じ動き
	if (boHpBarFrame <= 0) {//指定時間たったら
		color.w -= ColorWDec;
	}

	if (color.w <= Transparency) {
		boHpBarFrame = boHpBarFrameDef;//またこの時間分まつ
		color.w = 1.f;//一番明るい状態
	}

	boHpBarFrame = std::max(--boHpBarFrame, 0);//ToStartFrameの最小値は0

	sp_enemyhpbar->SetColor(color);
	sp_enemyhpbar->TransferVertexBuffer();
}

void CharParameters::boHpUpdate()
{
	boHpSizeChange();
	//色変え
	boHpColorPattern();

	//ボスHpの最大最小値。HPが負になったりバーが反対に飛び出ないように
	nowBossHP = std::clamp(nowBossHP, 0.f, bossMaxHP);

	//更新
	sp_enemyhpbar->Update();
	sp_enemyhpbarwaku->Update();
}

void CharParameters::Draw()
{
}

void CharParameters::boHpDraw()
{
	sp_enemyhpbar->Draw();
	sp_enemyhpbarwaku->Draw();
}

void CharParameters::DrawUI()
{
}