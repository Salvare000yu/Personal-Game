﻿#include <DirectXMath.h>
#include <vector>
#include <string>
#include<d3d12.h>
#include<wrl.h>
#include <d3dx12.h>

#pragma once
/// <summary>
/// 3dModel
/// </summary>
class Model
{
public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;//アンビエント係数
		float pad1;//パディング
		DirectX::XMFLOAT3 diffuse;//ディフューズ係数
		float pad2;//パディング
		DirectX::XMFLOAT3 specular;//スペキュラー係数
		float alpha;//アルファ
		DirectX::XMFLOAT2 tiling{ 1, 1 };//タイリング
		DirectX::XMFLOAT2 pad3{ 1, 1 };//パディング
		DirectX::XMFLOAT2 uvShift{ 0, 0 };//uvシフト
	};

	//マテリアル
	struct Material
	{
		std::string name;//マテリアル名
		DirectX::XMFLOAT3 ambient;//アンビエント影響度
		DirectX::XMFLOAT3 diffuse;//ディフューズ影響度
		DirectX::XMFLOAT3 specular;//スペキュラー影響度
		float alpha;//アルファ
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public://静的メンバ関数

	static Model* GetInstance();

	//OBJファイルから3Dモデルを読み込み
	static Model* LoadFromOBJ(const std::string& modelname);

	/// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	//描画コマンドリスト
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

	static void SetDevice(ID3D12Device* device) { Model::device = device; }

	void SetAlpha(float alpha) { this->material.alpha = alpha; }
	const float& GetAlpha() { return material.alpha; }

	void SetTiling(const DirectX::XMFLOAT2& tiling) { this->tiling = tiling; }
	const DirectX::XMFLOAT2& GetTiling() { return tiling; }

	void SetUvShift(const DirectX::XMFLOAT2& uvShift) { this->uvShift = uvShift; }
	const DirectX::XMFLOAT2& GetUvShift() { return uvShift; }

private:
	//借りるデバイス
	static ID3D12Device* device;

	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short>indices;

	Material material;

	DirectX::XMFLOAT2 tiling{ 1, 1 };
	DirectX::XMFLOAT2 uvShift{ 0, 0 };

	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	//定数バッファ（マテリアル）
	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffB1;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>vertBuff;
	//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>indexBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;

private:
	void LoadFromOBJInternal(const std::string& modelname);

	//デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	//各種バッファ生成
	void CreateBuffers();

	void TransferBuffers();
};
