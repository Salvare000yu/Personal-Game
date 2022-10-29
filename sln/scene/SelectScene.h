#pragma once
/// <summary>
/// セレクト画面
/// </summary>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include <memory>
#include "DxBase.h"

class SelectScene :public BaseScene
{
public:

	enum class SelectPattern {
		def,
		operationTOstart,
		operationTOtitle,
		startTOtitle,
		startTOoperation,
		titleTOoperation,
		titleTOstart,
	};

	SelectPattern selectPattern_ = SelectPattern::def;

	void Initialize() override;

	void SelectOper();

	void OperationWind();

	void SelectStart();

	void SelectTitle();

	void Finalize() override;

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	DxBase* dxBase = nullptr;
private:

	std::unique_ptr < Sprite> sp_stageselect = nullptr;
	std::unique_ptr < Sprite> sp_SSoper = nullptr;
	std::unique_ptr < Sprite> sp_SSstart = nullptr;
	std::unique_ptr < Sprite> sp_SStitle = nullptr;
	std::unique_ptr < Sprite> sp_SSNow = nullptr;
	std::unique_ptr < Sprite> sp_operation_wind = nullptr;

	//Model* model_1 = nullptr;

	//Object3d* object3d_1 = nullptr;

	bool VibFlag = false;

	int VibCount = 15;//タイトルから何フレーム振動させるか

	float NowSelect = 0;//0:操作説明（デフォ） 1:スタート　2:タイトルへ
	void  (SelectScene::* pFunc)();

	int WaitKeyEnter = 0;//Enterで操作説明がめん..
	bool OperWindOpenFlag = false;//開いていない

	float WaitKeyEase=0;//イーズのキー待機時間

	bool CursorMoveNowFlag = false;//移動中は何もするな false:動いてない

	//-----------セレクトシーン0から
	bool SSOp0_1Flag = false;//1に移動開始フラグ
	bool SSOp0_2Flag = false;//0から2に
	bool SSStart1_2Flag = false;//1から2に
	bool SSStart1_0Flag = false;//1から0に
	bool SStitle2_1Flag = false;//2から1に

	const float EaseValDef = 20;
	float EaseVal = EaseValDef;//イーズ値
	float DecEaseVal = 0.5;//イーズ値減らす値
};

