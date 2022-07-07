#include "PostEffect.h"
#include <d3dx12.h>
#include "WinApp.h"

using namespace DirectX;

PostEffect::PostEffect()
    : Sprite(
        100,					//テクスチャ番号
        { 0, 0 ,0},					//座標
        { 500.0f, 500.0f },		//サイズ
        { 1, 1, 1, 1 },			//色
        { 0.0f, 0.0f },			//アンカーポイント
        false,					//左右反転フラグ
        false)					//上下反転フラグ
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
    //// パイプラインステートの設定
    //commandList_->SetPipelineState(pipelineSet.pipelinestate.Get());
    //// ルートシグネチャの設定
    //commandList_->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
    //// プリミティブ形状を設定
    //commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //spriteBase->PreDraw();
    //spriteBase->PosteffectPreDraw();

    // テクスチャ用デスクリプタヒープの設定
    //commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() }; 
    //commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

    // 頂点バッファをセット
    commandList->IASetVertexBuffers(0, 1, &vbView_);

    // ルートパラメータ0番に定数バッファをセット
    commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

    // ルートパラメータ1番に シェーダリソースビューをセット
    spriteBase->SetGraphicsRootDescriptorTable(1, texNumber_);
    //commandList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

    // ポリゴンの描画（4頂点で四角形）
    commandList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::Initialize()
{
    HRESULT result = S_FALSE;

    SpriteBase* spriteBase = SpriteBase::GetInstance();

    //基底クラスとして初期化
    Sprite::Initialize(texNumber_,anchorpoint_, isFlipX_, isFlipY_);

    ////テクスチャリソース設定
    //D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
    //    DXGI_FORMAT_R8G8B8A8_UNORM,
    //    WinApp::window_width,
    //    (UINT)WinApp::window_height,
    //    1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    //);

    ////テクスチャバッファの生成
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
    //    //画像1行分のデータサイズ
    //    const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
    //    //画像全体のデータサイズ
    //    const UINT depthPitch = rowPitch * WinApp::window_height;
    //    //画像イメージ
    //    UINT* img = new UINT[pixelCount];
    //    for (int j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

    //    //テクスチャバッファにデータ転送
    //    result = texBuff_[texNumber_]->WriteToSubresource(0, nullptr,
    //        img, rowPitch, depthPitch);
    //    assert(SUCCEEDED(result));
    //    delete[] img;
    //}
    ////SRV用デスクリプタヒープ設定
    //D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
    //srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //srvDescHeapDesc.NumDescriptors = 1;
    ////SRV用デスクリプタヒープを生成
    //result = spriteBase->GetDevice()->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
    //assert(SUCCEEDED(result));

    ////SRV設定
    //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体
    //srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
    //srvDesc.Texture2D.MipLevels = 1;

    ////デスクリプターヒープにSRV作成
    //spriteBase->GetDevice()->CreateShaderResourceView(texBuff_[texNumber_].Get(), //ビューと関連付けるバッファ
    //    &srvDesc,
    //        descHeapSRV->GetCPUDescriptorHandleForHeapStart()
    //);
}
