#include "DebugText.h"

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

void DebugText::Initialize(SpriteBase* spriteBase, UINT texnumber)
{
	//nullptr
	assert(spriteBase);
	//引数メンバ変数に格納
	this->spriteBase_ = spriteBase;

	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites_); i++)
	{
		// スプライトを生成する
		sprites_[i] = Sprite::Create(texnumber, DirectX::XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 });
	}
}

void DebugText::Finalize()
{
	for (auto& sprite : sprites_) {
		delete sprite;
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	// 全ての文字について
	for (size_t i = 0, size = text.size(); i < size; ++i)
	{
		// 最大文字数超過
		if (spriteIndex_ >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		// ASCIIコードの2段分飛ばした番号を計算
		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		sprites_[spriteIndex_]->SetPosition({ x + fontWidth * scale * i, y, 0 });
		sprites_[spriteIndex_]->SetTexLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		sprites_[spriteIndex_]->SetTexSize({ fontWidth, fontHeight });
		sprites_[spriteIndex_]->SetSize({ fontWidth * scale, fontHeight * scale });
		// 頂点バッファ転送
		sprites_[spriteIndex_]->TransferVertexBuffer();
		// 更新
		sprites_[spriteIndex_]->Update();

		// 文字を１つ進める
		++spriteIndex_;
	}
}

// まとめて描画
void DebugText::DrawAll()
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex_; i++)
	{
		// スプライト描画
		sprites_[i]->Draw();
	}

	spriteIndex_ = 0;
}