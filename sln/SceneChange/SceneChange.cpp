#include "SceneChange.h"

SceneChange* SceneChange::GetInstance()
{
	static SceneChange instance;
	return &instance;
}

void SceneChange::Initialize()
{
	//sprite読み込み
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/SceneChange.png");
	// スプライトの生成
	sp_scenechange.reset(Sprite::Create(5, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
}

void SceneChange::Update()
{
	sp_scenechange->Update();
}

void SceneChange::Draw()
{
	sp_scenechange->Draw();
}
