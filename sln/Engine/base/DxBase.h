#pragma once
//directXcommon
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <chrono>

#include <DirectXMath.h>

//directX基盤
class DxBase
{
	DxBase(const DxBase& dxBase) = delete;
	DxBase& operator=(const DxBase& dxBase) = delete;

	DxBase() {};

public://メンバ関数
	static DxBase* GetInstance();

	void Initialize(WinApp* winApp);

	//描画前
	void PreDraw();
	//描画後処理
	void PostDraw();

	//デバイス取得
	ID3D12Device* GetDevice() { return device.Get(); }

	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

	inline float GetFps()const { return fps; }

private:
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory6>dxgiFactory;

	WinApp* winApp = nullptr;

	//direct3D系メンバ変数
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapforimgui;
	UINT64 fenceVal = 0;
	//------
	//バックバッファ
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>backBuffers;

	//FPS固定 時間記録
	std::chrono::steady_clock::time_point reference_;

	float fps = -1;
private:
	bool InitializeCommand();

	bool InitializeDevice();

	bool InitializeSwapchain();

	bool InitializeRenderTargetView();

	bool DxBase::InitializeDepthBuffer();

	bool DxBase::InitializeFence();

	bool DebugLayer();

	bool SetBreakOnSeverity();
	//----//FPS固定
	void InitFixFps();//初期化
	void UpdateFixFps();//更新
	//-----
};
