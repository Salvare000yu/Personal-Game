#include "SceneChangeDirection.h"

SceneChangeDirection* SceneChangeDirection::GetInstance()
{
	static SceneChangeDirection instance;
	return &instance;
}

void SceneChangeDirection::Initialize()
{
	//sprite�ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/SceneChange.png");
	// �X�v���C�g�̐���
	sp_scenechange.reset(Sprite::Create(5, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sp_scenechange->SetPosition({ 1280,0,0 });
}

void SceneChangeDirection::HideTheScreen()
{

	XMFLOAT3 pos = sp_scenechange->GetPosition();

	HideVel = -HideSp;//�E���獶�ɉB���Ă�

	pos.x += HideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x <= 0) {
		HideTheScreenFlag = false;//�B��������߂�
		//SceneChangeCompFlag = true;
	}
}

void SceneChangeDirection::Update()
{
	if (HideTheScreenFlag) {
		HideTheScreen();//��ʉB�������B���ŉB���J�n
	}

	sp_scenechange->Update();
}

void SceneChangeDirection::Draw()
{
	sp_scenechange->Draw();
}
