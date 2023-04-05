#include "SceneChangeDirection.h"
#include "DebugText.h"

SceneChangeDirection* SceneChangeDirection::GetInstance()
{
	static SceneChangeDirection instance;
	return &instance;
}

void SceneChangeDirection::Initialize()
{
	//sprite“Ç‚Ýž‚Ý
	SpriteBase::GetInstance()->LoadTexture(18, L"Resources/SceneChange.png");
	// ƒXƒvƒ‰ƒCƒg‚Ì¶¬
	sp_scenechange.reset(Sprite::Create(18, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sp_scenechange->SetPosition({ 1280,0,0 });

	SceneChangeDirectionFlag = false;
	SceneChangeCompFlag = false;
	HideTheScreenFlag = false;
	GameReadyStartFlag = false;
	OpenTheScreenFlag = false;
}

void SceneChangeDirection::HideTheScreen()
{
	XMFLOAT3 pos = sp_scenechange->GetPosition();

	HideVel = -HideSp;//‰E‚©‚ç¶‚É‰B‚µ‚Ä‚­

	pos.x += HideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x == 0) {
		HideTheScreenFlag = false;//‰B‚µ‚½‚©‚ç–ß‚·
		SceneChangeCompFlag = true;//‰B‚µI‚í‚è
	}
}
void SceneChangeDirection::OpenTheScreen()
{
	XMFLOAT3 pos = sp_scenechange->GetPosition();

	HideVel = HideSp;//‰E‚©‚ç¶‚É‰B‚µ‚Ä‚­

	pos.x += HideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x == 1280) {
		OpenTheScreenFlag = true;//Š®‘S‚ÉŠJ‚«Ø‚Á‚½
	}
}

void SceneChangeDirection::Update()
{
	if (HideTheScreenFlag) {
		HideTheScreen();//‰æ–Ê‰B‚·ðŒ’B¬‚Å‰B‚µŠJŽn
	}
	if (GameReadyStartFlag) {
		OpenTheScreen();//ƒV[ƒ“‘JˆÚŠ®—¹Œã‰æ–Ê‚ðŠJ‚¯‚é
	}

	sp_scenechange->Update();
}

void SceneChangeDirection::Draw()
{
	sp_scenechange->Draw();
}