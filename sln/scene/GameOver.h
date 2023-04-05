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

	float NowSelect = 0;//0:操作説明（デフォ）
	void  (GameOver::* pFunc)();

	float WaitKeyEase = 0;//イーズのキー待機時間

	bool CursorMoveNowFlag = false;//移動中は何もするな false:動いてない

	const float EaseValDef = 25;
	float EaseVal = EaseValDef;//イーズ値
	float DecEaseVal = 0.5;//イーズ値減らす値

	bool SSOp0_1Flag = false;//1に移動開始フラグ
	bool SSStart1_0Flag = false;//1から0に

	//Model* model_1 = nullptr;
	//Model* model_2 = nullptr;

	//Object3d* object3d_1 = nullptr;
	//Object3d* object3d_2 = nullptr;
	//Object3d* object3d_3 = nullptr;
};
