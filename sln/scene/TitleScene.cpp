#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
#include "GamePlayScene.h"
#include "SelectScene.h"
#include "CharParameters.h"

#include "Timer.h"

#include "PostEffect.h"

//PostEffect* postEffect = nullptr;

using namespace DirectX;

void TitleScene::Initialize()
{
#pragma region �`�揉��������

	WinApp* winApp = WinApp::GetInstance();

	CharParameters* charParameters = CharParameters::GetInstance();

	// �}�E�X�J�[�\����\��
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	camera.reset(new CameraTracking());
	Object3d::SetCamera(camera.get());

	////---obj���烂�f���f�[�^�ǂݍ���---
	mod_tunnel.reset(Model::LoadFromOBJ("tunnel"));
	mod_ground.reset(Model::LoadFromOBJ("ground"));
	mod_player.reset(Model::LoadFromOBJ("player"));
	mod_kaberight.reset(Model::LoadFromOBJ("Rkabetaijin"));
	mod_kabeleft.reset(Model::LoadFromOBJ("kabetaijin"));
	mod_logo.reset(Model::LoadFromOBJ("STRIKER_Logo"));
	////---3d�I�u�W�F�N�g����---
	obj_tunnel.reset(Object3d::Create());
	obj_ground.reset(Object3d::Create());
	obj_kaberight.reset(Object3d::Create());
	obj_kabeleft.reset(Object3d::Create());
	obj_logo.reset(Object3d::Create());
	////---3d�I�u�W�F�N�g��3d���f����R�Â���---
	obj_tunnel->SetModel(mod_tunnel.get());
	obj_ground->SetModel(mod_ground.get());
	obj_kaberight->SetModel(mod_kaberight.get());
	obj_kabeleft->SetModel(mod_kabeleft.get());
	obj_logo->SetModel(mod_logo.get());
	//------object3d�X�P�[��------//
	obj_tunnel->SetScale({ 100.0f, 40.0f, 40.0f });
	obj_ground->SetScale({ 80.0f, 20.0f, 500.0f });
	obj_kaberight->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_kabeleft->SetScale({ 40.0f, 40.0f, 40.0f });
	obj_logo->SetScale({ 40.f,40.f,40.f });
	//------object3d�ʒu------//
	obj_tunnel->SetPosition({ 0,40,-500 });
	obj_ground->SetPosition({ 0,-150,0 });
	obj_kaberight->SetPosition({ 490,340,-500 });
	obj_kabeleft->SetPosition({ -490,340,-500 });
	obj_logo->SetPosition({ 0,100,-1000 });
	//------object��]
	obj_tunnel->SetRotation({ 0,-90,0 });
	obj_kaberight->SetRotation({ 0,0,0 });
	obj_kabeleft->SetRotation({ 0,180,0 });
	obj_logo->SetRotation({ 0,0,0 });

	//���낢�됶��
	player_.reset(new Player());
	//���낢��L����������
	player_->Initialize();
	player_->SetPosition({ 0,150,-1950 });
	player_->SetModel(mod_player.get());

	player_->pAtkPossibleFlag = false;//�^�C�g���ł͒e��ł��Ȃ�

	//���@�o�ꉉ�o
	ApEndPPos = player_->GetPosition();
	ApStartPPos = ApEndPPos;
	ApStartPPos.z -= 500;//�������玩�@�̏����ʒu�܂Ŏw��t���[���|���ē���

	camera->SetTarget(player_->GetPosition());
	const float EyeXDef = 10;//�ŏI�ʒu
	const float EyeX = EyeXDef - (CamEyeMoveSpX * PApMoveFrameMax);//�ŏI�ʒu�[�i���@�o�ꎞ�ԁ����炷�l�j�@�o�ꎞ�ԕ����炷����
	camera->SetEye({ EyeX,160,-2000 });//�����ɃJ�����������āA�ŏ��̉��o�Ŏ��@��ǂ�������

	charParameters->Initialize();

	// �����ǂݍ���
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// �����Đ�
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// �X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/Title_oper.png");

	// �X�v���C�g�̐���
	sprite1.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_titleoper.reset(Sprite::Create(3, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//�X�v���C�g�|�W�V����


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
	////window�c���擾�������Ƃ��g��
	//WinApp* winApp = WinApp::GetInstance();

	//const float spAccel = 1.05f;//����

	////���S�����
	//XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	//XMFLOAT2 NameTexSize = sp_gametitlename->GetTexSize();

	//NamePosXCenter = (winApp->window_width / 2) - (NameTexSize.x / 2);//�摜����ɃZ�b�g����邩�璆�������
	//NamePosYCenter = (winApp->window_height / 2) - (NameTexSize.y / 2);//�V;

	//if (NamePos.x > NamePosXCenter)
	//{
	//	NamePos.x -= sp;
	//	sp *= spAccel;
	//}
	//else {
	//	MoveStartFlag = false;
	//}

	//sp_gametitlename->SetPosition({ NamePos });
	////XMFLOAT2 NameSize = sp_gametitlename->GetSize();
	////NameSize.x--;
	////NameSize.y--;
	////sp_gametitlename->SetSize({NameSize});
	////sp_gametitlename->TransferVertexBuffer();

	////sp_gametitlename->SetPosition({ NamePosXCenter,NamePosYCenter,NamePos.z });
	//sp_gametitlename->Update();

}

void TitleScene::PlayerAppear()
{

	XMFLOAT3 pos = player_->GetPosition();

	if (PMoveFrame < PApMoveFrameMax) {//�ő�t���[�����B�܂ł��

		float raito = (float)PMoveFrame / PApMoveFrameMax;
		PMoveFrame++;

		XMFLOAT3 pos{};
		pos.x = std::lerp(ApStartPPos.x, ApEndPPos.x, raito);
		pos.y = std::lerp(ApStartPPos.y, ApEndPPos.y, raito);
		pos.z = std::lerp(ApStartPPos.z, ApEndPPos.z, raito);
		player_->SetPosition(pos);

		XMFLOAT3 eyePos = camera->GetEye();
		eyePos.x += CamEyeMoveSpX;
		camera->SetEye(eyePos);

		camera->SetTarget(pos);
	}
	else {//�ő�t���[����
		PMoveFrame = PMoveFrameDef;//�V�[���؂�ւ��Ȃ��ł��g���̂Ńf�t�H���g�ɖ߂�
		ExitEndPPos = { pos.x,pos.y,ExitPosZ };//�ޏ�͎w��Z�܂ōs���Ă����
		ExitStartPPos = pos;//���ݎ��@���W����ޏ�n�߂�
		PAppearFlag = false;//�o�ꊮ��
	}

}
void TitleScene::DoorOpen()
{

	const int LDoorPosXRim = -2200;//���̕ǊJ���I���ꏊ
	const float DoorMoveSp = 7.2f;//�h�A���J�����x

	XMFLOAT3 LDoorPos = obj_kabeleft->GetPosition();
	XMFLOAT3 RDoorPos = obj_kaberight->GetPosition();

	//���̕ǂ����s������I���
	if (!(LDoorPos.x < LDoorPosXRim)) {
		LDoorPos.x -= DoorMoveSp;
		RDoorPos.x += DoorMoveSp;
	}
	else {
		DoorOpenFlag = true;
	}
	obj_kabeleft->SetPosition(LDoorPos);
	obj_kaberight->SetPosition(RDoorPos);
}
void TitleScene::SceneChange()
{
	//XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
	//const float StartSpAccel = 1.05f;

	//NamePos.x -= StartSp;
	//StartSp *= StartSpAccel;
	//sp_gametitlename->SetPosition({ NamePos });

	Input* input = Input::GetInstance();

	if (PMoveFrame < PExitMoveFrameMax) {//�ޏ�p���Ԃ����đޏꂷ��

		DoorOpen();//�����J����

		//�w�莞�Ԃ����U������
		if (--SceneChangeVibCount == 0) {
			input->PadVibrationDef();
		}

		float raito = (float)PMoveFrame / PExitMoveFrameMax;
		PMoveFrame++;

		XMFLOAT3 pos{};
		pos.x = std::lerp(ExitStartPPos.x, ExitEndPPos.x, raito);
		pos.y = std::lerp(ExitStartPPos.y, ExitEndPPos.y, raito);
		pos.z = std::lerp(ExitStartPPos.z, ExitEndPPos.z, raito);
		player_->SetPosition(pos);

		camera->SetTarget(pos);
	}
	else {//�ő�t���[����
		// ������~
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//�V�[���؂�ւ�
		BaseScene* scene = new SelectScene();
		sceneManager_->SetNextScene(scene);
	}

	//sp_gametitlename->Update();
}

void TitleScene::ToStartSprite()
{
	XMFLOAT4 color = sp_titleoper->GetColor();

	ToStartFrame--;//�����łȂ�����
	ToStartFrame = max(ToStartFrame, 0);

	if (ToStartFrame <= 0) {
		color.w -= 0.012f;
	}

	if (color.w <= 0.5f) {
		ToStartFrame = ToStartFrameDef;//�܂����̎��ԕ��܂�
		color.w = 1.f;
	}

	sp_titleoper->SetColor(color);
	sp_titleoper->TransferVertexBuffer();
	sp_titleoper->Update();

	{
		char tmp[32]{};
		sprintf_s(tmp, 32, "%2.f", (float)ToStartFrame);
		DebugText::GetInstance()->Print(tmp, 300, 390, 3);
	}
}

void TitleScene::UpDown()
{
	//XMFLOAT3 NamePos = sp_gametitlename->GetPosition();
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
	time = frame / 60;
	//NamePos.y += sinf(time * 6.f);
	frame++;
	//sp_gametitlename->SetPosition({ NamePos });
	//sp_gametitlename->Update();
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

	//sprite1->Update();
	if (PAppearFlag) {
		//BeforeUpdate(); 
		PlayerAppear();//���@�o��
	}

	//�o�ꊮ�����đޏ�O
	if (PAppearFlag == false && SceneChangeFlag == false)
	{
		if ((cInput->DecisionByEnter()))     // �X�y�[�X�L�[��������Ă�����
		{
			GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
			SceneChangeFlag = true;//�`�F���W�ړ��t���O���Ă�
			input->PadVibration();
		}

		ToStartSprite();
		UpDown();

		//postEffect->Update();
	}

	if (SceneChangeFlag) {
		SceneChange();//�`�F���W�ړ��J�n
	}

	camera->SetTarget(player_->GetPosition());//�J�����͎��@��ǂ�

	obj_tunnel->Update();
	obj_ground->Update();
	player_->Update();
	obj_kaberight->Update();
	obj_kabeleft->Update();
	obj_logo->Update();

	// �J�����̍X�V
	camera->Update();
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
	player_->Draw();
	obj_kaberight->Draw();
	obj_kabeleft->Draw();
	obj_logo->Draw();

	//3d�I�u�W�F�`��㏈��
	Object3d::PostDraw();

	//// �X�v���C�g���ʃR�}���h
	SpriteBase::GetInstance()->PreDraw();
	//// �X�v���C�g�`��
	//sprite1->Draw();

	//sp_gametitlename->Draw();

	if (PAppearFlag == false && SceneChangeFlag == false)//���@�o��I����Ă邩��ENTER�������O�Ȃ�
	{
		sp_titleoper->Draw();//ENTER�ŊJ�n�����I�摜
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

		//{
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "%2.f", player_->GetPosition().z);
	//	DebugText::GetInstance()->Print(tmp, 300, 390, 3);
	//}
}
