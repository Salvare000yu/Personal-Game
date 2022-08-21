//フレームワーク旧
#include "GameBase.h"
#include "PostEffect.h"
#include <memory>

void GameBase::Run()
{
	Initialize();

	while (true)  // ゲームループ
	{
		//毎フレーム処理
		Update();

		//終了リクエストで抜ける
		if (endReq_)
		{
			break;
		}
		//描画
		Draw();

	}
	//終了
	Finalize();
}

void GameBase::Initialize()
{

	// FbxManager* fbxManager = FbxManager::Create();
	 //windowsAPI初期化
	winApp = WinApp::GetInstance();
	winApp->Initialize();

	MSG msg{};  // メッセージ

#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理
	// DirectX初期化処理　ここから
   // HRESULT result;
	//DirectXの初期化
	dxBase = DxBase::GetInstance();
	dxBase->Initialize(winApp);

	// スプライト共通部分初期化
	spriteBase = SpriteBase::GetInstance();
	spriteBase->Initialize(dxBase->GetDevice(), dxBase->GetCmdList(), winApp->window_width, winApp->window_height);

	//const int SPRITES_NUM = 1;
	//Sprite sprites[SPRITES_NUM];

	  // デバッグテキスト
	debugText = DebugText::GetInstance();

	// デバッグテキスト用のテクスチャ番号を指定
	const int debugTextTexNumber = 0;
	// デバッグテキスト用のテクスチャ読み込み
	spriteBase->LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText->Initialize(spriteBase, debugTextTexNumber);

	camera = new Camera(WinApp::window_width, WinApp::window_height);

	//入力の初期化
	input = Input::GetInstance();
	//input->Initialize(winApp);
	//ゲームパッド
	input->ControllerInitialize(0);

	//オーディオの初期化
	gameSound = GameSound::GetInstance();
	gameSound->Initialize();

	//3dオブジェクト静的初期化
	Object3d::StaticInitialize(dxBase->GetDevice(), camera);

	//シーンマネージャーの生成
	sceneManager_ = new SceneManager();

	//fbx　初期化
	FbxLoader::GetInstance()->Initialize(dxBase->GetDevice());

	PostEffect::GetInstance()->Initialize();
}

void GameBase::Finalize()
{

	//シーンマネージャ解放
	delete sceneManager_;

	FbxLoader::GetInstance()->Finalize();

	//デバッグテキスト解放
	debugText->Finalize();

	//オーディオ解放
	gameSound->Finalize();

	//DirectX解放
	//delete dxBase;

	//windowsAPIの終了処理
	winApp->Finalize();

	//windowsAPI解放
	//delete winApp;
}

void GameBase::Update()
{
#pragma region ウィンドウメッセージ処理
	//// メッセージがある？
	//if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
	//    TranslateMessage(&msg); // キー入力メッセージの処理
	//    DispatchMessage(&msg); // プロシージャにメッセージを送る
	//}

	//// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	//if (msg.message == WM_QUIT) {
	//    break;
	//}
		//windowsのメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループ抜ける
		endReq_ = true;
		return;
	}
#pragma endregion ウィンドウメッセージ処理
	//入力更新
	input->Update();
	//ゲームパッド更新
	input->ControllerUpdate(0);

	//シーン更新
	sceneManager_->Update();/*

	PostEffect::GetInstance()->Update();*/
}

void GameBase::Draw()
{
#pragma region グラフィックスコマンド

	//シーン描画
	sceneManager_->Draw();
	//描画前処理
	dxBase->PreDraw();

	PostEffect::GetInstance()->Draw();

	spriteBase->PreDraw();
	// デバッグテキスト描画
	debugText->DrawAll();

	// ４．描画コマンドここまで

	//描画後処理
	dxBase->PostDraw();
}
