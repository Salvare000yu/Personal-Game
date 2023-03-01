#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"

#include "Timer.h"

#include "PostEffect.h"

//PostEffect* postEffect = nullptr;

using namespace DirectX;

void TitleScene::Initialize()
{
#pragma region �`�揉��������

	WinApp* winApp = WinApp::GetInstance();

	// �}�E�X�J�[�\����\��
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	////---obj���烂�f���f�[�^�ǂݍ���---
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	mod_ground.reset(Model::LoadFromOBJ("ground"));
	////---3d�I�u�W�F�N�g����---
	obj_tunnel.reset(Object3d::Create());
	obj_ground.reset(Object3d::Create());
	////---3d�I�u�W�F�N�g��3d���f����R�Â���---
	obj_tunnel->SetModel(mod_tunnel.get());
	obj_ground->SetModel(mod_ground.get());
	//------object3d�X�P�[��------//
	obj_tunnel->SetScale({ 100.0f, 40.0f, 40.0f });
	obj_ground->SetScale({ 80.0f, 20.0f, 500.0f });
	//------object3d�ʒu------//
	obj_tunnel->SetPosition({ 0,40,2000 });
	obj_ground->SetPosition({ 0,-150,0 });
	//------object��]------//
	obj_tunnel->SetRotation({ 0,-90,0 });

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3D�I�u�W�F�N�g�̐�
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/GameTitleName.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/Title_oper.png");

	// �X�v���C�g�̐���
	sprite1.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_gametitlename.reset(Sprite::Create(2, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_titleoper.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//�X�v���C�g�|�W�V����
	sp_gametitlename->SetPosition({ winApp->window_width,NamePosYCenter,0 });

	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // �X�v���C�g�̍��W�ύX
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);
	//}

	//�|�X�g�G�t�F�N�g�p�e�N�X�`���ǂݍ���
	//SpriteBase::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	//�|�X�g�G�t�F�N�g������
//	postEffect = new PostEffect();
////	postEffect->Initialize();//100, { 0.0f, 0.0f }, false, false��������
//	postEffect->SetSize({ 500.0f,500.0f });
//	postEffect->SetPosition({ 10,10,0 });
//	postEffect->TransferVertexBuffer();
#pragma endregion �`�揉��������
}

void TitleScene::Finalize()
{
//	delete postEffect;
}

void TitleScene::BeforeUpdate()
{
	//window�c���擾�������Ƃ��g��
	WinApp* winApp = WinApp::GetInstance();

	const float spAccel = 1.05f;//����

	//���S�����
	XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	XMFLOAT2 NameTexSize = sp_gametitlename->GetTexSize();

	NamePosXCenter = (winApp->window_width / 2) - (NameTexSize.x / 2);//�摜����ɃZ�b�g����邩�璆�������
	NamePosYCenter = (winApp->window_height / 2) - (NameTexSize.y / 2);//�V;

	if (NamePos.x > NamePosXCenter)
	{
		NamePos.x -= sp;
		sp *= spAccel;
	}
	else { 
		MoveStartFlag = false; 
	}

	sp_gametitlename->SetPosition({ NamePos });
	//XMFLOAT2 NameSize = sp_gametitlename->GetSize();
	//NameSize.x--;
	//NameSize.y--;
	//sp_gametitlename->SetSize({NameSize});
	//sp_gametitlename->TransferVertexBuffer();

	//sp_gametitlename->SetPosition({ NamePosXCenter,NamePosYCenter,NamePos.z });
	sp_gametitlename->Update();
}

void TitleScene::SceneChange()
{
	XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	const float StartSpAccel = 1.05f;

	NamePos.x -= StartSp;
	StartSp *= StartSpAccel;
	sp_gametitlename->SetPosition({ NamePos });

	Input* input = Input::GetInstance();

	if (--SceneChangeVibCount == 0) {
		input->PadVibrationDef();
	}
	if (NamePos.x < -1280) {
		// ������~
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}

	sp_gametitlename->Update();
}

void TitleScene::UpDown()
{
	XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	//NamePosYUpDown*=0.99;
	//switch (upDownPattern_)
	//{
	//case UpDownPattern::def:
	//	NamePos.y += NamePosYUpDown;
	//	if(NamePos.y>= NamePosMoveMax){ 
	//		NamePosYUpDown = NamePosYUpDownDef;//�f�t�H���g�l�ɖ߂�
	//		upDownPattern_ = UpDownPattern::Down; 
	//	}
	//	break;
	//case UpDownPattern::Up:
	//	NamePos.y += NamePosYUpDown;
	//	if (NamePos.y >= NamePosMoveMax) { 
	//		NamePosYUpDown = NamePosYUpDownDef;//�f�t�H���g�l�ɖ߂�
	//		upDownPattern_ = UpDownPattern::Down; 
	//	}
	//	break;
	//case UpDownPattern::Down:
	//	NamePos.y -= NamePosYUpDown;
	//	if(NamePos.y<=NamePosMoveMin){
	//		NamePosYUpDown = NamePosYUpDownDef;//�f�t�H���g�l�ɖ߂�
	//		upDownPattern_ = UpDownPattern::Up; 
	//	}
	//	break;
	time=frame / 60;
	NamePos.y += sinf(time*6.f);
	frame++;
	sp_gametitlename->SetPosition({ NamePos });
	sp_gametitlename->Update();
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	////------------------------�f�o�b�O�p�I�I�I�I�I�I
	const bool InputSPACE = input->PushKey(DIK_SPACE);
	const bool InputENTER = input->PushKey(DIK_RETURN);
	if (InputSPACE && InputENTER) {
		// ������~
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}
	////------------------------�f�o�b�O�p�I�I�I�I�I�I

	//�Z���N�g����U������������
	if (--VibCount == 0) {
		input->PadVibrationDef();
	}

	sprite1->Update();
	if (MoveStartFlag == true) { BeforeUpdate(); }

	if (MoveStartFlag == false && SceneChangeFlag == false)
	{
		
		//�������u�Ԃ̂�
		//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
		//const bool TriggerEnter = input->TriggerKey(DIK_RETURN);
		//�p�b�h�������u��
		//const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));

		if ((cInput->DecisionByEnter()))     // �X�y�[�X�L�[��������Ă�����
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			SceneChangeFlag = true;//�`�F���W�ړ��t���O���Ă�
			input->PadVibration();
		}

		UpDown();

		//postEffect->Update();
	}

	if (SceneChangeFlag == true) {
		SceneChange();//�`�F���W�ړ��J�n
	}

	obj_tunnel->Update();
	obj_ground->Update();

	DrawUI();
}

void TitleScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();
	//3d�I�u�W�F�`��O����
	Object3d::PreDraw(DxBase::GetInstance()->GetCmdList());

	//�RD�I�u�W�F�N�gDraw
	obj_tunnel->Draw();
	obj_ground->Draw();

	//3d�I�u�W�F�`��㏈��
	Object3d::PostDraw();

	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//// �X�v���C�g�`��
	//sprite1->Draw();

	sp_gametitlename->Draw();

	if (MoveStartFlag == false && SceneChangeFlag == false)
	{
		sp_titleoper->Draw();
	}

}

void TitleScene::DrawUI()
{
	//DebugText::GetInstance()->Print("[ENTERorGAMEPAD:A] PLAYSCENE", 300, 100, 3.0f);
	//if (MoveStartFlag == true) { DebugText::GetInstance()->Print("MoveStartF:true", 300, 100, 3.0f); }
	//else { DebugText::GetInstance()->Print("MoveStartF:false", 300, 100, 3.0f); }

	//if (SceneChangeFlag == true) { DebugText::GetInstance()->Print("SceneChangeF:true", 300, 200, 3.0f);
	//}
	//else { DebugText::GetInstance()->Print("SceneChangeF:false", 300, 200, 3.0f); }

}
