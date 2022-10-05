#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"

#include <DirectXMath.h>

void Boss::ApproachInit()
{
	//攻撃用カウント初期化して間隔代入すれば一旦待ってから発射可能
	AtkCount = AtkInterval;
}

void Boss::Attack()
{
	//キー入力使う
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.5);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossBullet> madeBullet = std::make_unique<BossBullet>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet->Initialize();
	madeBullet->SetModel(eBulModel);
	madeBullet->SetPosition(position);

	//弾登録
	bullets_.push_back(std::move(madeBullet));
}

void Boss::Initialize()
{

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 20.0f, 20.0f, 20.0f });
	//場所
	obj->SetPosition({ -100,50,50 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");

	//近づくパターン初期化
	ApproachInit();
}

void Boss::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;

	//if (input3) {
	//	XMFLOAT3 position = obj->GetPosition();
	//	position.z += 5;
	//	obj->SetPosition(position);
	//}

		//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return !bullet->GetAlive();
		});

	//黄金の回転
	for (int i = 0; i < 1; i++)
	{
		frame += 1.f;

		XMFLOAT3 rotation = obj->GetRotation();
		rotation.y += 0.7f;
		rotation.x += 0.4f;
		obj->SetRotation({ rotation });

		XMFLOAT3 position = obj->GetPosition();
		//position.x += 5.f * sin(time * 3.14159265358f);
		obj->SetPosition(position);
	}
	//----------------------------------------------↓関数化しろボケ
	switch (actionPattern_) {
	case ActionPattern::Approach://近づくパターン
	default:
		//---突撃---//
		//発射カウントをデクリメント
		AtkCount--;
		//時が満ちたら
		if (AtkCount == 0) {
			//突撃時、生存時のみ発射
			if (alive) { Attack(); }
			//再びカウントできるように初期化
			AtkCount = AtkInterval;
		}

		//敵の移動
		XMFLOAT3 position = obj->GetPosition();
		position.z -= ApproachSp;
		position.y += ApproachSp;
		position.x += 3.f * sinf(time * 3.14159265358f);
		obj->SetPosition(position);

		//ある程度近づいたらキモ過ぎて離れる
		if (position.z == ApproachLim) {
			actionPattern_ = ActionPattern::Leave;
		}
		break;
	case ActionPattern::Leave://後退パターン
		//---後退---//
		XMFLOAT3 positionBack = obj->GetPosition();
		positionBack.z += ApproachSp;
		positionBack.y -= ApproachSp;
		obj->SetPosition(positionBack);

		//ある程度離れたら近づいてくる
		if (positionBack.z == LeaveLim) {
			actionPattern_ = ActionPattern::Approach;
		}
		break;
	}
	//----------------------------------------------↑関数化しろボケ

	//弾更新
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}

	obj->Update();
}

void Boss::Draw()
{
	//弾
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive) {
		obj->Draw();
	}
}
