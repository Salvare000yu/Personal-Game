#pragma once

#include "PipelineSet.h"

#include <DirectXTex.h>

/// <summary>
/// �X�v���C�g���ʕ���
/// </summary>
class SpriteBase
{
public:

    static SpriteBase* GetInstance();
    // �e�N�X�`���̍ő喇��
    static const int spriteSRVCount = 512;

    /// <summary>
    /// �������F�f�o�C�X�@�R�}���h���X�g�@�E�B���h�E�����c��
    /// </summary>
    void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList,int window_width, int window_height);

    /// <summary>
    /// �`��O����
    /// </summary>
    void PreDraw();

    /// <summary>
    /// �`��O����
    /// </summary>
    void PosteffectPreDraw();

    /// <summary>
    /// �e�N�X�`���ǂݍ���
    /// </summary>
    /// <param name="texnumber"></param>
    /// <param name="filename"></param>
    void LoadTexture(UINT texnumber, const wchar_t* filename);

    /// <summary>
    /// ���[�g�f�X�N���v�^�e�[�u���̐ݒ�
    /// </summary>
    /// <param name="rootparameterIndex">���[�g�p�����[�^�ԍ�</param>
    /// <param name="texNumber">�e�N�X�`���ԍ�</param>
    void SetGraphicsRootDescriptorTable(UINT rootparameterIndex,UINT texNumber);

    const DirectX::XMMATRIX& GetMatProjection() { return matProjection_; }

    /// <summary>
    /// �e�N�X�`���擾
    /// </summary>
    /// <param name="texnumber"></param>
    /// <returns></returns>
    ID3D12Resource* GetTexBuff(int texNumber);

    //
    ID3D12Device* GetDevice() { return dev; }

    ID3D12GraphicsCommandList* GetCommandList() { return commandList_; }


//private:
protected:
    // �p�C�v���C���Z�b�g
    PipelineSet pipelineSet;
    // �ˉe�s��
    DirectX::XMMATRIX matProjection_{};
    // �e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
    // �e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_[spriteSRVCount];

    //---������Ȃ�����comptr�g��Ȃ�
    //�؂��f�o�C�X
    ID3D12Device* dev=nullptr;
    //�؂��R�}���h���X�g
    ID3D12GraphicsCommandList* commandList_=nullptr;
    //---

//private:
protected:
    //�X�v���C�g�p�p�C�v���C������
    void CreateGraphicsPipeline();


};

