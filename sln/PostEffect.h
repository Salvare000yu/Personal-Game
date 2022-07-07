#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "PipelineSet.h"

// Microsoft::WRL::���ȗ�
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
// DirectX::���ȗ�
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;

class PostEffect :
	public Sprite
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw();

		/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	//ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }

		//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff_[2];
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

private:
	//�؂��R�}���h���X�g
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
};