#include "Sprite.h"
#include <d3dx12.h>

using namespace DirectX;

SpriteBase* Sprite::GetInstance()
{
	return nullptr;
}

Sprite* Sprite::Create(UINT texNumber, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size,
	DirectX::XMFLOAT4 color, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	//SpriteBase* spriteBase = SpriteBase::GetInstance();

	Sprite* instance = new Sprite(texNumber, position, size, color, anchorpoint, isFlipX, isFlipY);
	//インスタンス初期化
	instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY);

	return instance;
}

Sprite::Sprite(UINT texNumber, DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 size,
	DirectX::XMFLOAT4 color, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	this->position_ = position;
	this->size_ = size;
	this->anchorpoint_ = anchorpoint;
	this->matWorld_ = XMMatrixIdentity();
	this->color_ = color;
	this->texNumber_ = texNumber;
	this->isFlipX_ = isFlipX;
	this->isFlipY_ = isFlipY;
	this->texSize_ = size;
	this->isInvisible = false;
}

void Sprite::Initialize(UINT texNumber, DirectX::XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;
	//-----------------------メンバ変数にカキコ
	// テクスチャ番号をコピー
	texNumber_ = texNumber;
	// アンカーポイントをコピー
	anchorpoint_ = anchorpoint;
	// 反転フラグをコピー
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	SpriteBase* spriteBase = SpriteBase::GetInstance();

	// 頂点データ
	VertexPosUv vertices[4];

	// 指定番号の画像が読み込み済みなら
	if (spriteBase->GetTexBuff(texNumber_)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteBase->GetTexBuff(texNumber_)->GetDesc();

		// スプライトの大きさを画像の解像度に合わせる
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 頂点バッファ生成
	result = spriteBase->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));

	// 頂点バッファデータ転送
	TransferVertexBuffer();

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices);
	vbView_.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	result = spriteBase->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
	constMap->mat = spriteBase->GetMatProjection();
	constBuff_->Unmap(0, nullptr);
}

void Sprite::TransferVertexBuffer()
{
	SpriteBase* spriteBase = SpriteBase::GetInstance();

	HRESULT result = S_FALSE;

	// 頂点データ
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // 左下
		{{}, {0.0f, 0.0f}}, // 左上
		{{}, {1.0f, 1.0f}}, // 右下
		{{}, {1.0f, 0.0f}}, // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	if (isFlipX_)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// 左右入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // 左下
	vertices[LT].pos = { left, top,     0.0f }; // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top,    0.0f }; // 右上

	// 指定番号の画像が読み込み済みなら
	if (spriteBase->GetTexBuff(texNumber_)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = spriteBase->GetTexBuff(texNumber_)->GetDesc();

		float tex_left = texLeftTop_.x / resDesc.Width;
		float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;
		float tex_top = texLeftTop_.y / resDesc.Height;
		float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,   tex_top }; // 左上
		vertices[RB].uv = { tex_right,  tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,  tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff_->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	if (isInvisible) {
		return;
	}

	SpriteBase* spriteBase = SpriteBase::GetInstance();

	ID3D12GraphicsCommandList* commandList = spriteBase->GetCommandList();

	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	// ルートパラメータ0番に定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// ルートパラメータ1番に シェーダリソースビューをセット
	spriteBase->SetGraphicsRootDescriptorTable(1, texNumber_);

	// ポリゴンの描画（4頂点で四角形）
	commandList->DrawInstanced(4, 1, 0, 0);
}

void Sprite::Update()
{
	SpriteBase* spriteBase = SpriteBase::GetInstance();

	// ワールド行列の更新
	matWorld_ = XMMatrixIdentity();
	// Z軸回転
	matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));
	// 平行移動
	matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld_ * spriteBase->GetMatProjection();
	constMap->color = color_;
	constBuff_->Unmap(0, nullptr);
}