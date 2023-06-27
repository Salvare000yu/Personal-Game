#include "SceneChangeDirection.h"
#include "DebugText.h"

using namespace DirectX;

SceneChangeDirection* SceneChangeDirection::GetInstance()
{
	static SceneChangeDirection instance;
	return &instance;
}

void SceneChangeDirection::Initialize()
{
	//sprite読み込み
	SpriteBase::GetInstance()->LoadTexture(13, L"Resources/SceneChange.png");
	// スプライトの生成
	sp_scenechange.reset(Sprite::Create(13, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sp_scenechange->SetPosition({ 1280,0,0 });

}

void SceneChangeDirection::HideTheScreen()
{
	XMFLOAT3 pos = sp_scenechange->GetPosition();

	hideVel = -hideSp;//右から左に隠してく

	pos.x += hideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x == 0) {
		hideTheScreenFlag = false;//隠したから戻す
		sceneChangeCompFlag = true;//隠し終わり
	}
}
void SceneChangeDirection::OpenTheScreen()
{
	XMFLOAT3 pos = sp_scenechange->GetPosition();

	hideVel = hideSp;//右から左に隠してく

	pos.x += hideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x == 1280) {
		openTheScreenFlag = true;//完全に開き切った
	}
}

void SceneChangeDirection::Update()
{
	if (hideTheScreenFlag) {
		HideTheScreen();//画面隠す条件達成で隠し開始
	}
	if (gameReadyStartFlag) {
		OpenTheScreen();//シーン遷移完了後画面を開ける
	}

	sp_scenechange->Update();
}

void SceneChangeDirection::Draw()
{
	sp_scenechange->Draw();
}