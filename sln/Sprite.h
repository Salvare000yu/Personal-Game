#pragma once

#include"SpriteBase.h"

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

/// <summary>
/// Sprite���\���N���X
/// </summary>
class Sprite
{

public:
	/// <summary>
/// �R���X�g���N�^
/// </summary>
	Sprite(UINT texNumber, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color,
		DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	static SpriteBase* GetInstance();

	// ���_�f�[�^
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		DirectX::XMFLOAT4 color; // �F (RGBA)
		DirectX::XMMATRIX mat;   // �R�c�ϊ��s��
	};

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">x���]</param>
	/// <param name="isFlipY">y���]</param>
	static Sprite* Create(UINT texNumber, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size, DirectX::XMFLOAT4 color,
		DirectX::XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	//������
	void Initialize(UINT texNumber, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// ���_�o�b�t�@�]��
	/// </summary>
	void TransferVertexBuffer();

	/// <summary>
	/// 
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();

	/// ���W�̎擾
	const DirectX::XMFLOAT3& GetPosition() { return position_; }

	const DirectX::XMFLOAT2& GetSize() { return size_; }

	void SetPosition(const DirectX::XMFLOAT3& position) { position_ = position; }
	void SetRotation(float rotation) { rotation_ = rotation; }
	void SetSize(const DirectX::XMFLOAT2& size) { size_ = size; }
	void SetTexLeftTop(const DirectX::XMFLOAT2& texLeftTop) { texLeftTop_ = texLeftTop; }
	void SetTexSize(const DirectX::XMFLOAT2& texSize) { texSize_ = texSize; }

//private:
protected:
	//���_�o�b�t�@;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	//���_�o�b�t�@�r���[;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//�萔�o�b�t�@;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// Z�����̉�]�p
	float rotation_ = 0.0f;
	// ���W
	DirectX::XMFLOAT3 position_ = { 0,0,0 };
	// ���[���h�s��
	DirectX::XMMATRIX matWorld_;
	// �F(RGBA)
	DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };
	// �e�N�X�`���ԍ�
	UINT texNumber_ = 0;
	// �傫��
	DirectX::XMFLOAT2 size_ = { 100, 100 };
	// �A���J�[�|�C���g
	DirectX::XMFLOAT2 anchorpoint_ = { 0.5f, 0.5f };
	// ���E���]
	bool isFlipX_ = false;
	// �㉺���]
	bool isFlipY_ = false;
	// �e�N�X�`��������W
	DirectX::XMFLOAT2 texLeftTop_ = { 0, 0 };
	// �e�N�X�`���؂�o���T�C�Y
	DirectX::XMFLOAT2 texSize_ = { 100, 100 };
	// ��\��
	bool isInvisible = false;

};

