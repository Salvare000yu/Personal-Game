#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"
#include "BaseScene.h"

class Pause
{
public:
	static Pause* GetInstance();

	void Initialize();
	void EveryInit();
	//ポーズしているとき
	void PauseNow();
	//タイトルへ
	void Update();
	void Draw();
	void DrawUI();
	void Finalize();
	void PauseGoTitle();
	void SpUpdate();
	void SpOpenPauseDraw();
	void SpFlagTrueNowDraw();
	void SpOperWindDraw();
	//操作説明画面を開いている
	void OperationWind();
	//プレイ続行
	void PauseConti();
	//操作説明画面を開く
	void PauseOper();

	//-------------↓げったーせったー↓--------------//
	//ポーズしてるかどうか
	void SetPauseFlag(bool pauseFlag) { this->pauseFlag = pauseFlag; }
	const bool& GetPauseFlag() { return pauseFlag; }
	//操作説明画面開いているかどうか
	void SetOpWindOpenFlag(bool operWindOpenFlag) { this->operWindOpenFlag = operWindOpenFlag; }
	const bool& GetOpWindOpenFlag() { return operWindOpenFlag; }
	//タイトル戻るフラグ
	void SetSceneChangeTitleFlag(bool sceneChangeTitleFlag) { this->sceneChangeTitleFlag = sceneChangeTitleFlag; }
	const bool& GetSceneChangeTitleFlag() { return sceneChangeTitleFlag; }
	//-------------↑げったーせったー↑--------------//

	int WaitKeyP = 0;//０だけでポーズ開閉できるように

private:
	std::unique_ptr < Sprite> sp_openpause = nullptr;
	std::unique_ptr < Sprite> sp_pause = nullptr;
	std::unique_ptr < Sprite> sp_operation = nullptr;
	std::unique_ptr < Sprite> sp_operation_wind = nullptr;
	std::unique_ptr < Sprite> sp_continuation = nullptr;
	std::unique_ptr < Sprite> sp_gotitle = nullptr;

	bool pauseFlag = false;//ポーズしていない
	const float pauseSelectSizeDef = 300.0f;
	const float pauseSelectSize = 350.0f;
	float pauseNowSelect = 0;//0:続ける 1:操作説明　2:タイトル
	void  (Pause::* pFunc)();

	int waitKeyEnter = 0;//Enterで操作説明がめん..
	bool operWindOpenFlag = false;//開いていない

	bool sceneChangeTitleFlag = false;//タイトルへ

};
