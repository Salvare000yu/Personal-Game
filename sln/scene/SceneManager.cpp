#include "SceneManager.h"

#include "../PostEffect.h"
#include "../DxBase.h"
#include "DebugText.h"

SceneManager::~SceneManager()
{
	//最後のシーン終了と解放
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update()
{
	//シーン切り替えあったら
	if (nextScene_) {
		if (scene_) {
			//旧シーン終了
			scene_->Finalize();
			delete scene_;
		}

		scene_ = nextScene_;
		nextScene_ = nullptr;

		//シーンマネージャセット
		scene_->SetSceneManager(this);

		//新シーンの初期化
		scene_->Initialize();
	}
	scene_->Update();
}

void SceneManager::Draw()
{
	PostEffect::GetInstance()->PreDrawScene(DxBase::GetInstance()->GetCmdList());
	scene_->Draw();
	PostEffect::GetInstance()->PostDrawScene(DxBase::GetInstance()->GetCmdList());
}
