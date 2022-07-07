#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "PipelineSet.h"

// Microsoft::WRL::を省略
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
// DirectX::を省略
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

class PostEffect :
	public Sprite
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw();

		/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	//ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }

		//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff_[2];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

private:
	//借りるコマンドリスト
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	// テクスチャ用デスクリプタヒープの生成
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// パイプラインセット
	PipelineSet pipelineSet;
};