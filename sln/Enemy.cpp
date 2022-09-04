#include "Enemy.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"

#include <DirectXMath.h>

void Enemy::ApproachInit()
{
	//攻撃用カウント初期化して間隔代入すれば一旦待ってから発射可能
	AtkCount = AtkInterval;
}

Enemy* Enemy::GetInstance()
{
	static Enemy instance;

	return &instance;
}

void Enemy::OnCollision()
{
}

void Enemy::Attack()
{
	//キー入力使う
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.5);

	//弾発射
	XMFLOAT3 position = obj_enemy->GetPosition();
	//弾生成
	std::unique_ptr<EnemyBullet> madeBullet = std::make_unique<EnemyBullet>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet->Initialize({ position });

	//弾登録
	bullets_.push_back(std::move(madeBullet));
}

void Enemy::Initialize()
{

	//もでる読み込み
	mod_enemy.reset(Model::LoadFromOBJ("bullet2"));
	//作る
	obj_enemy.reset(Object3d::Create());
	//セット
	obj_enemy->SetModel(mod_enemy.get());
	//-----↓任意↓-----//
	//大きさ
	obj_enemy->SetScale({ 20.0f, 20.0f, 20.0f });
	//場所
	obj_enemy->SetPosition({ -100,50,50 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");

	//近づくパターン初期化
	ApproachInit();
}

void Enemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}

		//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsVanish();
		});

	//黄金の回転
	for (int i = 0; i < 1; i++)
	{
		frame += 1.f;

		XMFLOAT3 rotation = obj_enemy->GetRotation();
		rotation.y += 0.7f;
		rotation.x += 0.4f;
		obj_enemy->SetRotation({ rotation });

		XMFLOAT3 position = obj_enemy->GetPosition();
		//position.x += 5.f * sin(time * 3.14159265358f);
		obj_enemy->SetPosition(position);
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
			//突撃時のみ発射
			Attack();
			//再びカウントできるように初期化
			AtkCount = AtkInterval;
		}

		//弾の移動
		XMFLOAT3 position = obj_enemy->GetPosition();
		position.z -= ApproachSp;
		position.y += ApproachSp;
		position.x += 3.f * sin(time * 3.14159265358f);
		obj_enemy->SetPosition(position);

		//ある程度近づいたらキモ過ぎて離れる
		if (position.z == ApproachLim) {
			actionPattern_ = ActionPattern::Leave;
		}
		break;
	case ActionPattern::Leave://後退パターン
		//---後退---//
		XMFLOAT3 positionBack = obj_enemy->GetPosition();
		positionBack.z += ApproachSp;
		positionBack.y -= ApproachSp;
		obj_enemy->SetPosition(positionBack);

		//ある程度離れたら近づいてくる
		if (positionBack.z == LeaveLim) {
			actionPattern_ = ActionPattern::Approach;
		}
		break;
	}
	//----------------------------------------------↑関数化しろボケ

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	obj_enemy->Update();
}

void Enemy::Draw()
{
	//弾
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	obj_enemy->Draw();
}
