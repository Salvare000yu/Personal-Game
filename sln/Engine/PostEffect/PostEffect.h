#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "PipelineSet.h"

class PostEffect
{
public:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	static PostEffect* GetInstance();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	// 頂点データ
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData {
		float time;
		DirectX::XMFLOAT2 windowSize;  // 画面サイズ
		float vignettePow;
	};

	void CreateGraphicsPipelineState();

	void TransfarConstBuffer();

	// パイプラインセット
	PipelineSet pipelineSet;

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

public:
	//ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }

	//シーン描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* commandList);
	//シーン描画後処理
	void PostDrawScene(ID3D12GraphicsCommandList* commandList);

	static void SetDevice(ID3D12Device* device);

	//画面クリアカラー
	static const float clearColor[4];

	static const UINT texBuffNum = 2;

	// ヴィネットの強さ
	inline void SetVignettePow(float vignettePow) { this->vignettePow = vignettePow; }

private:

	static ID3D12Device* device_;

	//頂点バッファ;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	//頂点バッファビュー;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//定数バッファ;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	// テクスチャ番号
	UINT texNumber_ = 0;
	// 大きさ
	DirectX::XMFLOAT2 size_ = { 100, 100 };

	// テクスチャ切り出しサイズ
	DirectX::XMFLOAT2 texSize_ = { 100, 100 };

	//テクスチャバッファ

	ComPtr<ID3D12Resource> texBuff_[texBuffNum];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//深度バッファ
	ComPtr<ID3D12Resource>depthBuff;
	//RTV用デスクリプタテーブル
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapDSV;
	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState>piplineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature>rootSignature;

	//借りるコマンドリスト
	//ID3D12GraphicsCommandList* commandList_ = nullptr;

	// テクスチャ用デスクリプタヒープの生成
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// パイプラインセット
	//PipelineSet pipelineSet;

	float frame = 0;
	float vignettePow = 0.f;
};