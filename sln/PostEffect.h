#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "PipelineSet.h"



class PostEffect
{
public:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	static PostEffect* GetInstance();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	// ���_�f�[�^
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		float time;
	};

	void CreateGraphicsPipelineState();

	void TransfarConstBuffer();

	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

public:
	//ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }

	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* commandList);
	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* commandList);

	static void SetDevice(ID3D12Device* device);

	//��ʃN���A�J���[
	static const float clearColor[4];

	static const UINT texBuffNum = 2;

private:

	static ID3D12Device* device_;

	//���_�o�b�t�@;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;

	//���_�o�b�t�@�r���[;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//�萔�o�b�t�@;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

	// �e�N�X�`���ԍ�
	UINT texNumber_ = 0;
	// �傫��
	DirectX::XMFLOAT2 size_ = { 100, 100 };

	// �e�N�X�`���؂�o���T�C�Y
	DirectX::XMFLOAT2 texSize_ = { 100, 100 };

	//�e�N�X�`���o�b�t�@

	ComPtr<ID3D12Resource> texBuff_[texBuffNum];
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource>depthBuff;
	//RTV�p�f�X�N���v�^�e�[�u��
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapDSV;
	//�O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState>piplineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature>rootSignature;

	//�؂��R�}���h���X�g
	//ID3D12GraphicsCommandList* commandList_ = nullptr;

	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// �p�C�v���C���Z�b�g
	//PipelineSet pipelineSet;

	float frame = 0;
};