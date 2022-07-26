//directXcommon
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include "imgui/imgui.h"

#pragma once

//directX基盤
class DxBase
{

	DxBase(const DxBase& dxBase) = delete;
	DxBase& operator=(const DxBase& dxBase) = delete;

	DxBase() {};

public://メンバ関数
	static DxBase* GetInstance();

	void Initialize(WinApp* winApp);

	/// <summary>
	/// imgui初期化
	/// </summary>
	/// <returns>成否</returns>
	bool InitializeImgui();

	//描画前
	void PreDraw();
	//描画後処理
	void PostDraw();

	//デバイス取得
	ID3D12Device* GetDevice() { return device.Get(); }

	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

private:
	//デバイス
	//Microsoft::WRL::ComPtr<ID3D12Device>dev;
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
private:
	bool InitializeCommand();

	bool InitializeDevice();

	bool InitializeSwapchain();

	bool InitializeRenderTargetView();

	bool DxBase::InitializeDepthBuffer();

	bool DxBase::InitializeFence();

	bool DebugLayer();

	bool SetBreakOnSeverity();
};

