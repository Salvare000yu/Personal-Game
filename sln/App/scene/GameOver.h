#pragma once
/// <summary>
/// タイトル画面
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include <memory>

class GameOver :public BaseScene
{
public:

	enum class SelectPattern {
		def,
		goTitle,
		retry,
	};
	SelectPattern selectPattern_ = SelectPattern::def;

	void Initialize() override;

	void Finalize() override;

	void Retry();

	void GoTitle();

	void Update() override;

	void Draw() override;

	void DrawUI() override;

private:

	std::unique_ptr < Sprite> gameover = nullptr;
	std::unique_ptr < Sprite> sp_gotitle = nullptr;
	std::unique_ptr < Sprite> sp_retry = nullptr;
	std::unique_ptr < Sprite> sp_Now = nullptr;

	float nowSelect = 0;//0:操作説明（デフォ）
	void  (GameOver::* pFunc)();

	float waitKeyEase = 0;//イーズのキー待機時間

	bool cursorMoveNowFlag = false;//移動中は何もするな false:動いてない

	const float easeValDef = 25;
	float easeVal = easeValDef;//イーズ値
	float decEaseVal = 0.5;//イーズ値減らす値

	bool sSOp0_1Flag = false;//1に移動開始フラグ
	bool sSStart1_0Flag = false;//1から0に

};
