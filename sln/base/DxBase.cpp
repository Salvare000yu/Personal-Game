﻿//基盤部分DirectXcommon

#include "DxBase.h"

#include <cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace DirectX;

using namespace Microsoft::WRL;

DxBase* DxBase::GetInstance()
{
	static DxBase dxCom{};
	return &dxCom;
}

void DxBase::Initialize(WinApp* winApp)
{
	//null検出
	assert(winApp);

	//メンバ変数に記録
	this->winApp = winApp;

	//デバッグレイヤー　デバイス生成より上
	DebugLayer();

	//デバイス生成
	InitializeDevice();

	//GPUValidation　デバイス生成より下
	SetBreakOnSeverity();

	//コマンド系初期化
	InitializeCommand();
	//スワップチェーン初期化
	InitializeSwapchain();
	//レンダーターゲットビュー
	InitializeRenderTargetView();
	//深度バッファ
	InitializeDepthBuffer();
	//フェンス
	InitializeFence();

	//FPS固定
	InitFixFps();
}

void DxBase::PreDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// １．リソースバリアで書き込み可能に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// ２．描画先指定
  // レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// ３．画面クリア           R     G     B    A
	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
	//レンダーターゲットクリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポート領域の設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));
	// シザー矩形の設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));
}

void DxBase::PostDraw()
{
	// バックバッファの番号を取得
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// ５．リソースバリアを戻す
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	cmdList->Close();
	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get() }; // コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ（裏表の入替え）
	swapchain->Present(1, 0);

	// コマンドキューの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//FPS固定更新 VSyncを残し追加で待つので実行完了待ち直後に
	UpdateFixFps();

	cmdAllocator->Reset(); // キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
#pragma endregion グラフィックスコマンド
}

bool DxBase::InitializeDevice()
{
	HRESULT result;

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif
	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // 動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description;   // アダプター名
		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];   // 採用
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	return true;
}

bool DxBase::InitializeCommand()
{
	HRESULT result;
	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));

	// コマンドリストを生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	return true;
}

bool DxBase::InitializeSwapchain()
{
	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;

	// スワップチェーンの生成
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		winApp->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1.As(&swapchain);

	return true;
}

bool DxBase::InitializeRenderTargetView()
{
	HRESULT result;

	// 各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	heapDesc.NumDescriptors = 2;
	heapDesc.Type =
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	device->CreateDescriptorHeap(&heapDesc,
		IID_PPV_ARGS(&rtvHeaps));

	//表裏2つ分のポインタ
	backBuffers.resize(2);

	//表裏二つ分
	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				device->GetDescriptorHandleIncrementSize(heapDesc.Type)
			)
		);
	}
	return true;
}

bool DxBase::InitializeDepthBuffer()
{
	HRESULT result;

	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	// 深度バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool DxBase::InitializeFence()
{
	HRESULT result = S_FALSE;

	// フェンスの生成
	UINT64 fenceVal = 0;

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

bool DxBase::DebugLayer()
{
#ifdef _DEBUG
	//デバックレイヤーをオン
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	//GPUValidation
  //  debugController->SetEnableGPUBasedValidation(TRUE);

	return true;
}

bool DxBase::SetBreakOnSeverity()
{
#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);//重要エラー止まる的な？
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);//エラー止まる
		//--comptr--infoQueue->Release();
	}

	D3D12_MESSAGE_ID denyIds[] = {
		//windows11でdxgiデバッグレイヤーとdx12デバッグレイヤーのそうごっさようばぐによるエラーメッセージ
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//抑制表示レベル
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	//指定したエラー表示抑制
	infoQueue->PushStorageFilter(&filter);

#endif

	return true;
}

void DxBase::InitFixFps()
{
	//現在時間記録
	reference_ = std::chrono::steady_clock::now();
}

void DxBase::UpdateFixFps()
{
	//1/60ジャスト
	constexpr std::chrono::microseconds kMinTime(uint64_t(1'000'000.0f / 60.0f));
	//1/60よりちょっとだけ短い時間
	constexpr std::chrono::microseconds kMinCheckTime(uint64_t(1'000'000.0f / 65.0f));

	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回記録からの経過時間取得
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60(よりわずかに短い時間）経過前なら
	if (elapsed < kMinCheckTime) {
		//1/60秒経過するまで微小なスリープ繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			//1マイクロスリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//現在時間記録
	reference_ = std::chrono::steady_clock::now();

	fps = 1'000'000.0f / elapsed.count();
}