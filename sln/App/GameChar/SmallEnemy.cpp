#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include "GameSound.h"

#include <DirectXMath.h>
#include <random>

using namespace DirectX;

void SmallEnemy::Attack()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.1f);

	//弾発射
	XMFLOAT3 sePos = obj->GetPosition();
	//弾生成
	std::unique_ptr<SmallEnemyBullet> madeBullet = std::make_unique<SmallEnemyBullet>();
	madeBullet->Initialize();
	madeBullet->SetModel(seBulModel);
	madeBullet->SetPosition(sePos);

	//弾登録
	bullets_.push_back(std::move(madeBullet));
}

void SmallEnemy::Initialize()
{
	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 8.0f, 8.0f, 8.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });

	atkCount = atkInterval;
}

void SmallEnemy::Retire()
{
	if (isRetire) {
		//自機より右にいるか左にいるかでどちらに捌けるか変わる
		if (obj->GetPosition().x < shotTag->GetPosition().x) {
			retirePat_ = RetirePat::Left;
			//左パターンへ
			isRetire = false;
		}
		else {
			retirePat_ = RetirePat::Right;
			//右パターンへ
			isRetire = false;
		}
	}

	XMFLOAT3 sePos = obj->GetPosition();
	constexpr float retireSp = 3.f;
	if (retirePat_ == RetirePat::Right) {
		sePos.x += retireSp;
		if (sePos.x >= 700) {
			alive = false;;//消滅
		}
	}
	if (retirePat_ == RetirePat::Left) {
		sePos.x -= retireSp;
		if (sePos.x <= -700) {
			alive = false;//消滅
		}
	}
	obj->SetPosition(sePos);
}

void SmallEnemy::BulletUpdate()
{
	//弾更新
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {
		//その時のターゲット座標
		//一度きり
		if (bullet->OnlyFlag) {
			bullet->ShotTagMoment = shotTag->GetPosition();
			//最初の位置
			bullet->sePosMoment = obj->GetPosition();
			bullet->OnlyFlag = false;
		}

		bullet->Update();

		bullet->Nowframe++;
		bullet->MoveSp.x = (bullet->ShotTagMoment.x - bullet->sePosMoment.x);
		bullet->MoveSp.y = (bullet->ShotTagMoment.y - bullet->sePosMoment.y);
		bullet->MoveSp.z = (bullet->ShotTagMoment.z - bullet->sePosMoment.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&bullet->MoveSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に(速度)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 10.f);
		// FLOAT3に変換
		XMStoreFloat3(&bullet->MoveSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		bullet->NowPos.x = bullet->sePosMoment.x + bullet->MoveSp.x * bullet->Nowframe;
		bullet->NowPos.y = bullet->sePosMoment.y + bullet->MoveSp.y * bullet->Nowframe;
		bullet->NowPos.z = bullet->sePosMoment.z + bullet->MoveSp.z * bullet->Nowframe;

		bullet->SetPosition(bullet->NowPos);//その時の位置
	}
}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);
	constexpr float moveSp = 7.f;

	//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<SmallEnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});

	XMFLOAT3 sePos = obj->GetPosition();
	//登場は奥から向かってくる
	if (isSeApproach) {
		if (obj->GetPosition().z > shotTag->GetPosition().z + posZMax) {
			sePos.z -= moveSp;
			obj->SetPosition(sePos);
		}
		else {
			//滞在時間
			if (--retireFrame < 0) {
				//捌け開始
				isRetire = true;
				isSeApproach = false;
			}
		}
	}

	Retire();

	//時が満ちたら
	if (--atkCount == 0) {
		//生存時のみ発射
		if (alive) {
			Attack();
		}
		//再びカウントできるように初期化
		atkCount = atkInterval;
	}

	//弾更新
	BulletUpdate();

	obj->Update();
}

void SmallEnemy::Draw()
{
	//弾描画
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive) {
		obj->Draw();
	}
}