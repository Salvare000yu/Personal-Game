#pragma once

#include "BaseScene.h"
/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
public:
	~SceneManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//次シーン予約
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	BaseScene* GetScene() { return scene_; }

private:
	//nowシーン
	BaseScene* scene_ = nullptr;
	//next
	BaseScene* nextScene_ = nullptr;
};
