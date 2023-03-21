#include "SceneChangeDirection.h"
#include "DebugText.h"

SceneChangeDirection* SceneChangeDirection::GetInstance()
{
	static SceneChangeDirection instance;
	return &instance;
}

void SceneChangeDirection::Initialize()
{
	//sprite�ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(18, L"Resources/SceneChange.png");
	// �X�v���C�g�̐���
	sp_scenechange.reset(Sprite::Create(18, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sp_scenechange->SetPosition({ 1280,0,0 });

	SceneChangeDirectionFlag = false;
	SceneChangeCompFlag = false;
	HideTheScreenFlag = false;
	GameReadyStartFlag = false;

}

void SceneChangeDirection::HideTheScreen()
{

	XMFLOAT3 pos = sp_scenechange->GetPosition();

	HideVel = -HideSp;//�E���獶�ɉB���Ă�

	pos.x += HideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x == 0) {
		HideTheScreenFlag = false;//�B��������߂�
		SceneChangeCompFlag = true;//�B���I���
	}
}
void SceneChangeDirection::OpenTheScreen()
{

	XMFLOAT3 pos = sp_scenechange->GetPosition();

	HideVel = HideSp;//�E���獶�ɉB���Ă�

	pos.x += HideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x <= 0) {

	}
}

void SceneChangeDirection::Update()
{
	if (HideTheScreenFlag) {
		HideTheScreen();//��ʉB�������B���ŉB���J�n
	}
	if (GameReadyStartFlag) {
		OpenTheScreen();//�V�[���J�ڊ������ʂ��J����
	}
	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", sp_scenechange->GetPosition().x);
		DebugText::GetInstance()->Print(tmp, 430, 430, 3);
	}
	sp_scenechange->Update();
}

void SceneChangeDirection::Draw()
{
	sp_scenechange->Draw();
}
