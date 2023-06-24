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
	//ｰｰ色

	//パラメータ関連初期化
	bossDefense = bossDefenseDef;
	pNextPlaceGoFlag = true;

}

void CharParameters::Update()
{
}

void CharParameters::Draw()
{
}

void CharParameters::DrawUI()
{
}