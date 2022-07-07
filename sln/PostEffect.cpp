#include "PostEffect.h"
#include <d3dx12.h>
#include "WinApp.h"

using namespace DirectX;

PostEffect::PostEffect()
    : Sprite(
        100,					//�e�N�X�`���ԍ�
        { 0, 0 ,0},					//���W
        { 500.0f, 500.0f },		//�T�C�Y
        { 1, 1, 1, 1 },			//�F
        { 0.0f, 0.0f },			//�A���J�[�|�C���g
        false,					//���E���]�t���O
        false)					//�㉺���]�t���O
{
}

void PostEffect::Draw()
{
    if (isInvisible) {
        return;
    }

    SpriteBase* spriteBase = SpriteBase::GetInstance();

    ID3D12GraphicsCommandList* commandList = spriteBase->GetCommandList();



    ////commandList_ = commandList;
    //// �p�C�v���C���X�e�[�g�̐ݒ�
    //commandList_->SetPipelineState(pipelineSet.pipelinestate.Get());
    //// ���[�g�V�O�l�`���̐ݒ�
    //commandList_->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
    //// �v���~�e�B�u�`���ݒ�
    //commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //spriteBase->PreDraw();
    //spriteBase->PosteffectPreDraw();

    // �e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
    //commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() }; 
    //commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

    // ���_�o�b�t�@���Z�b�g
    commandList->IASetVertexBuffers(0, 1, &vbView_);

    // ���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
    commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

    // ���[�g�p�����[�^1�Ԃ� �V�F�[�_���\�[�X�r���[���Z�b�g
    spriteBase->SetGraphicsRootDescriptorTable(1, texNumber_);
    //commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

    // �|���S���̕`��i4���_�Ŏl�p�`�j
    commandList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::Initialize()
{
    HRESULT result = S_FALSE;

    SpriteBase* spriteBase = SpriteBase::GetInstance();

    //���N���X�Ƃ��ď�����
    Sprite::Initialize(texNumber_,anchorpoint_, isFlipX_, isFlipY_);

    ////�e�N�X�`�����\�[�X�ݒ�
    //D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
    //    DXGI_FORMAT_R8G8B8A8_UNORM,
    //    WinApp::window_width,
    //    (UINT)WinApp::window_height,
    //    1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    //);

    ////�e�N�X�`���o�b�t�@�̐���
    //result = spriteBase->GetDevice()->CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
    //        D3D12_MEMORY_POOL_L0),
    //    D3D12_HEAP_FLAG_NONE,
    //    &texresDesc,
    //    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
    //    nullptr,
    //    IID_PPV_ARGS(&texBuff_[texNumber_]));
    //assert(SUCCEEDED(result));
    //{
    //    const UINT pixelCount = WinApp::window_width * WinApp::window_height;
    //    //�摜1�s���̃f�[�^�T�C�Y
    //    const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
    //    //�摜�S�̂̃f�[�^�T�C�Y
    //    const UINT depthPitch = rowPitch * WinApp::window_height;
    //    //�摜�C���[�W
    //    UINT* img = new UINT[pixelCount];
    //    for (int j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

    //    //�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
    //    result = texBuff_[texNumber_]->WriteToSubresource(0, nullptr,
    //        img, rowPitch, depthPitch);
    //    assert(SUCCEEDED(result));
    //    delete[] img;
    //}
    ////SRV�p�f�X�N���v�^�q�[�v�ݒ�
    //D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    //srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //srvDescHeapDesc.NumDescriptors = 1;
    ////SRV�p�f�X�N���v�^�q�[�v�𐶐�
    //result = spriteBase->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
    //assert(SUCCEEDED(result));

    ////SRV�ݒ�
    //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //�ݒ�\����
    //srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
    //srvDesc.Texture2D.MipLevels = 1;

    ////�f�X�N���v�^�[�q�[�v��SRV�쐬
    //spriteBase->GetDevice()->CreateShaderResourceView(texBuff_[texNumber_].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
    //    &srvDesc,
    //        descHeapSRV->GetCPUDescriptorHandleForHeapStart()
    //);
}
