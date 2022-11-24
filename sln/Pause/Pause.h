#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"
#include "BaseScene.h"

class Pause :public BaseScene, public BaseObject
{
public:
	static Pause* GetInstance();

	void Initialize()override;
	void EveryInit();
	//ポーズしているとき
	void PauseNow();
	//タイトルへ
	void Update()override;
	void Draw()override;
	void DrawUI()override;
	void Finalize()override;
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
	void SetPauseFlag(bool PauseFlag) { this->PauseFlag = PauseFlag; }
	const bool& GetPauseFlag() { return PauseFlag; }
	//操作説明画面開いているかどうか
	void SetOpWindOpenFlag(bool OperWindOpenFlag) { this->OperWindOpenFlag = OperWindOpenFlag; }
	const bool& GetOpWindOpenFlag() { return OperWindOpenFlag; }
	//タイトル戻るフラグ
	void SetSceneChangeTitleFlag(bool SceneChangeTitleFlag) { this->SceneChangeTitleFlag = SceneChangeTitleFlag; }
	const bool& GetSceneChangeTitleFlag() { return SceneChangeTitleFlag; }
	//-------------↑げったーせったー↑--------------//

	int WaitKey0 = 0;//０だけでポーズ開閉できるように
private:
	std::unique_ptr < Sprite> sp_openpause = nullptr;
	std::unique_ptr < Sprite> sp_pause = nullptr;
	std::unique_ptr < Sprite> sp_operation = nullptr;
	std::unique_ptr < Sprite> sp_operation_wind = nullptr;
	std::unique_ptr < Sprite> sp_continuation = nullptr;
	std::unique_ptr < Sprite> sp_gotitle = nullptr;

	bool PauseFlag = false;//ポーズしていない
	const float PauseSelectSizeDef = 300.0f;
	const float PauseSelectSize = 350.0f;
	float PauseNowSelect = 0;//0:続ける 1:操作説明　2:タイトル
	void  (Pause::* pFunc)();

	int WaitKeyEnter = 0;//Enterで操作説明がめん..
	bool OperWindOpenFlag = false;//開いていない

	bool SceneChangeTitleFlag = false;//タイトルへ
};

