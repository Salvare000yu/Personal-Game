#include "GamePlayScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "FbxLoader.h"
#include "DxBase.h"
#include "EndScene.h"
#include "FbxObject3d.h"
#include "safe_delete.h"

#include <DirectXMath.h>

using namespace DirectX;

void GamePlayScene::Initialize()
{
#pragma region 描画初期化処理

	camera.reset(new Camera(WinApp::window_width, WinApp::window_height));

	camera->SetTarget({ 0,50,-200 });
	camera->SetEye({ 0,48,-210 });

	//デバイスをセット
	FbxObject3d::SetDevice(DxBase::GetInstance()->GetDevice());
	// カメラセット
	Object3d::SetCamera(camera.get());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	FbxObject3d::SetCamera(camera.get());

	//使う定義とか　仮おいとくね
	time = frame / 60.f;	// 60fps想定

	const float playerRotZ = 0.f;

	//------objからモデルデータ読み込み---
	model_1.reset(Model::LoadFromOBJ("ground"));
	mod_worlddome.reset(Model::LoadFromOBJ("skydome"));
	mod_player.reset(Model::LoadFromOBJ("hiyoko"));
	//Model* model_3 = Model::LoadFromOBJ("chr_sword");
	//------3dオブジェクト生成------//
	object3d_1.reset(Object3d::Create());
	obj_worlddome.reset(Object3d::Create());
	obj_player.reset(Object3d::Create());
	//------3dオブジェクトに3dモデルを紐づける------//
	object3d_1->SetModel(model_1.get());
	obj_worlddome->SetModel(mod_worlddome.get());
	obj_player->SetModel(mod_player.get());
	//------object3dスケール------//
	object3d_1->SetScale({ 100.0f, 20.0f, 500.0f });
	obj_worlddome->SetScale({ 5.0f, 5.0f, 5.0f });
	obj_player->SetScale({ 2.0f, 2.0f, 2.0f });
	//------object3d位置------//
	object3d_1->SetPosition({ 0,-1,0 });
	obj_worlddome->SetPosition({ 0,200,0 });
	obj_player->SetPosition({ 0,43,-170 });
	//------object回転------//
	//obj_player->SetRotation({ 0,0,40 });

	fbxModel_1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//----------FBX オブジェクト生成とモデルのセット-----------//

	fbxObject_1 = new FbxObject3d;

	fbxObject_1->Initialize();

	//------fbxセット------//
	fbxObject_1->SetModel(fbxModel_1);

	//------fbxスケール------//
	fbxObject_1->SetScale({ 10.0f, 10.0f, 10.0f });

	//------fbx位置------//
	fbxObject_1->SetPosition({ 0,24,100 });

	fbxObject_1->PlayAnimation();

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("E_rhythmaze_128.wav");
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("E_rhythmaze_128.wav");
	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/play.png");
	SpriteBase::GetInstance()->LoadTexture(2, L"Resources/target_guide.png");

	// スプライトの生成
	sprite_back.reset(Sprite::Create(1, DirectX::XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_guide.reset(Sprite::Create(2, DirectX::XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sprite_back->TransferVertexBuffer();
	//sp_guide->TransferVertexBuffer();

	//sprite_back.push_back(sprite_back);
	//スプライトポジション
	sprite_back->SetPosition({ -11400,0,0 });
	sp_guide->SetPosition({ 400,200,0 });

	//スプライトサイズ
	//XMFLOAT2 size = sp_guide->GetSize();
	//sp_guide->GetSize();
	//size.x=90;
	//sp_guide->SetSize({200,0});

	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // スプライトの座標変更
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);
	//}

	//fbx モデル名指定してファイル読み込み
	//FbxLoader::GetInstance()->LoadModelFromFile(
	//	"cube"
	//);

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター
}

void GamePlayScene::Finalize()
{
	safe_delete(fbxObject_1);
	safe_delete(fbxModel_1);
}

void GamePlayScene::Update()
{
	//トリガーキー使う
	bool TriggerKey(UINT index);

	Input* input = Input::GetInstance();

	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

	if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		// 画面クリアカラーの数値を書き換える
		clearColor[1] = 1.0f;
	}

	// 座標操作
	const bool inputUp = input->PushKey(DIK_UP);
	const bool inputDown = input->PushKey(DIK_DOWN);
	const bool inputRight = input->PushKey(DIK_RIGHT);
	const bool inputLeft = input->PushKey(DIK_LEFT);

	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	const bool inputT = input->PushKey(DIK_T);
	const bool inputE = input->PushKey(DIK_E);

	const bool TriggerJ = input->TriggerKey(DIK_J);
	const bool TriggerM = input->TriggerKey(DIK_M);
	const bool TriggerK = input->TriggerKey(DIK_K);
	const bool TriggerE = input->TriggerKey(DIK_E);
	const bool Trigger0 = input->TriggerKey(DIK_0);
	const bool Trigger1 = input->TriggerKey(DIK_1);
	const bool Trigger2 = input->TriggerKey(DIK_2);

	if (inputW || inputS || inputA || inputD || inputQ || inputZ)
	{

		//------プレイヤーも同じ移動------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 1;

		if (inputS) {
			// カメラをバックさせる
			camera->MoveEyeVector(XMFLOAT3(0, 0, -moveSpeed));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z - moveSpeed));

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x <= 10) {
				rotation.x += 1.f;
			}
			obj_player->SetRotation(rotation);

		}
		if (inputW) {
			// カメラを前進させる
			camera->MoveEyeVector(XMFLOAT3(0, 0, +moveSpeed));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z + moveSpeed));

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x >= -10) {
				rotation.x -= 1.f;
			}
			obj_player->SetRotation(rotation);

		}
		if (inputA) {
			// カメラを左進させる
			camera->MoveEyeVector(XMFLOAT3(-moveSpeed, 0, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x - moveSpeed, camera->GetTarget().y, camera->GetTarget().z));

			XMFLOAT3 position = obj_player->GetPosition();
			position.x = position.x - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;

		}/*
		if(!inputA&&OldInputFlag_A==TRUE) {
			XMFLOAT3 rotation = obj_player->GetRotation();
			rotation.z = 0;
			obj_player->SetRotation(rotation);

			OldInputFlag_A = FALSE;
		}*/

		if (inputD) {
			// カメラを右進させる
			camera->MoveEyeVector(XMFLOAT3(moveSpeed, 0, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x + moveSpeed, camera->GetTarget().y, camera->GetTarget().z));

			XMFLOAT3 position = obj_player->GetPosition();
			position.x = position.x + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z >= -10) {
			rotation.z -= 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;

		}
		//else{ OldInputFlag = FALSE; }

		//if (OldInputFlag == TRUE) {

		//	XMFLOAT3 rotation = obj_player->GetRotation();
		//	rotation.z = 0;
		//	obj_player->SetRotation(rotation);

		//	OldInputFlag = FALSE;
		//}

		if (inputQ) {
			// カメラを上昇させる
			camera->MoveEyeVector(XMFLOAT3(0, moveSpeed, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y + moveSpeed, camera->GetTarget().z));

			XMFLOAT3 position = obj_player->GetPosition();
			position.y = position.y + moveSpeed;
			obj_player->SetPosition(position);
		}
		if (inputZ) {
			// カメラを下降させる
			camera->MoveEyeVector(XMFLOAT3(0, -moveSpeed, 0));
			camera->SetTarget(XMFLOAT3(camera->GetTarget().x, camera->GetTarget().y - moveSpeed, camera->GetTarget().z));

			XMFLOAT3 position = obj_player->GetPosition();
			position.y = position.y - moveSpeed;
			obj_player->SetPosition(position);
		}
	}

	//タゲ移動
	if (inputUp || inputDown || inputRight || inputLeft)
	{
		constexpr float tagmove = 5;
		if (inputUp) {
			XMFLOAT3 position = sp_guide->GetPosition();
			position.y-=tagmove;
			sp_guide->SetPosition({ position });
		}
		if (inputDown) {
			XMFLOAT3 position = sp_guide->GetPosition();
			position.y+= tagmove;
			sp_guide->SetPosition({ position });
		}
		if (inputRight) {
			XMFLOAT3 position = sp_guide->GetPosition();
			position.x+= tagmove;
			sp_guide->SetPosition({ position });
		}
		if (inputLeft) {
			XMFLOAT3 position = sp_guide->GetPosition();
			position.x-= tagmove;
			sp_guide->SetPosition({ position });
		}
	}

	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 rotation = obj_worlddome->GetRotation();
		rotation.y+=0.3f;
		obj_worlddome->SetRotation({ rotation });
	}

	if (Trigger0)     // スペースキーが押されていたら
	{
		//シーン切り替え
		BaseScene* scene = new EndScene();
		sceneManager_->SetNextScene(scene);
	}
	if (inputE)
	{
		XMFLOAT3 rotation = obj_player->GetRotation();
		rotation.y++;
		obj_player->SetRotation(rotation);
	}

	//バックスプライト動
	//SPmove SPbackmoveobj;
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 position = sprite_back->GetPosition();

		position.x += 5;
		if (position.x == 0) { position.x = -11400; }

		sprite_back->SetPosition(position);
	}

	DebugText::GetInstance()->Print("PLAY", 200, 100);
	camera->Update();

	//3dobjUPDATE
	object3d_1->Update();
	obj_worlddome->Update();
	obj_player->Update();

	// FBX Update
	fbxObject_1->Update();

	//スプライト更新
	sprite_back->Update();
	sp_guide->Update();
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DxBase::GetInstance()->GetCmdList();

	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	//sprite_back->Draw();
	

	//3dオブジェ描画前処理
	Object3d::PreDraw(DxBase::GetInstance()->GetCmdList());

	//3dオブジェ描画
	object3d_1->Draw();
	obj_worlddome->Draw();
	obj_player->Draw();

	// FBX3dオブジェクト描画
	//fbxObject_1->Draw(cmdList);

	//パーティクル描画
	//particleMan->Draw(cmdList);

	//3dオブジェ描画後処理
	Object3d::PostDraw();

	// ４．描画コマンドここから

	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//    DrawObject3d(&object3ds[i], dxBase->GetCmdList(), basicDescHeap.Get(), vbView, ibView,
	//        CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeap->GetGPUDescriptorHandleForHeapStart(), constantBufferNum, dxBase->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)),
	//        indices, _countof(indices));
	//}

	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw(); 
	
	//------お手前スプライト描画
	sp_guide->Draw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
   // sprite->Draw();

}

void GamePlayScene::DrawUI()
{
}