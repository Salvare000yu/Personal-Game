﻿#include "SmallEnemy.h"
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
	std::unique_ptr<SmallEnemyBullet>& madeBullet =
		bullets_.emplace_front(std::make_unique<SmallEnemyBullet>());
	madeBullet->Initialize();
	madeBullet->SetModel(seBulModel);
	madeBullet->SetPosition(sePos);
}

void SmallEnemy::Initialize()
{
	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 8.0f, 8.0f, 8.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });

	//初回行動　登場開始
	smallEnemyActionPattern = std::bind(&SmallEnemy::StartAppear, this);

	atkCount = atkInterval;
}

void SmallEnemy::Shot()
{
	//時が満ちたら
	if (--atkCount == 0) {
		//生存時のみ発射
		if (alive) {
			Attack();
		}
		//再びカウントできるように初期化
		atkCount = atkInterval;
	}
}

void SmallEnemy::StartAppear()
{
	XMFLOAT3 sePos = obj->GetPosition();
	//登場は奥から向かってくる
	if (obj->GetPosition().z > shotTag->GetPosition().z + posZMax) {
		constexpr float moveSp = 7.f;
		sePos.z -= moveSp;
		obj->SetPosition(sePos);
	}
	else {
		//滞在時間
		if (--retireFrame < 0) {
			//捌け開始
			//自機より右にいるか左にいるかでどちらに捌けるか変わる
			if (obj->GetPosition().x < shotTag->GetPosition().x) {
				//左パターンへ
				smallEnemyActionPattern = std::bind(&SmallEnemy::RetireLeft, this);
			}
			else
				//右パターンへ
				smallEnemyActionPattern = std::bind(&SmallEnemy::RetireRight, this);
		}
	}

	Shot();//弾をうつ
}

void SmallEnemy::Exit()
{
	XMFLOAT4 seColor = obj->GetColor();
	constexpr float seColDecVal = 0.05f;//透明にする速度
	seColor.w -= seColDecVal;
	obj->SetColor(seColor);

	if (seColor.w <= 0) {
		alive = false;//消滅
	}
}

void SmallEnemy::RetireRight()
{//右に捌ける
	XMFLOAT3 sePos = obj->GetPosition();
	sePos.x += retireSp+2.f;
	obj->SetPosition(sePos);

	//傾ける　Playerのように
	XMFLOAT3 seRot = obj->GetRotation();
	if (seRot.x >= -rotMax) {
		seRot.x -= rotSp;
		obj->SetRotation(seRot);
	}

	if (sePos.x >= 700) {
		Exit();//消える
	}
}
void SmallEnemy::RetireLeft()
{//左に捌ける
	XMFLOAT3 sePos = obj->GetPosition();

	//傾ける　Playerのように
	XMFLOAT3 seRot = obj->GetRotation();
	if (seRot.x <= rotMax) {
		seRot.x += rotSp;
		obj->SetRotation(seRot);
	}

	sePos.x -= retireSp+ 2.f;
	if (sePos.x <= -700) {
		Exit();//消える
	}
	obj->SetPosition(sePos);
}

void SmallEnemy::BulletUpdate()
{
	//弾更新
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {
		//その時のターゲット座標
		//一度きり
		if (bullet->onlyFlag) {
			bullet->shotTagMoment = shotTag->GetPosition();
			//最初の位置
			bullet->sePosMoment = obj->GetPosition();
			bullet->onlyFlag = false;
		}

		bullet->Update();

		bullet->nowFrame++;
		bullet->moveSp.x = (bullet->shotTagMoment.x - bullet->sePosMoment.x);
		bullet->moveSp.y = (bullet->shotTagMoment.y - bullet->sePosMoment.y);
		bullet->moveSp.z = (bullet->shotTagMoment.z - bullet->sePosMoment.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&bullet->moveSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に(速度)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 10.f);
		// FLOAT3に変換
		XMStoreFloat3(&bullet->moveSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		bullet->nowPos.x = bullet->sePosMoment.x + bullet->moveSp.x * bullet->nowFrame;
		bullet->nowPos.y = bullet->sePosMoment.y + bullet->moveSp.y * bullet->nowFrame;
		bullet->nowPos.z = bullet->sePosMoment.z + bullet->moveSp.z * bullet->nowFrame;

		bullet->SetPosition(bullet->nowPos);//その時の位置
	}
}

void SmallEnemy::Update()
{
	//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<SmallEnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});

	//弾更新
	BulletUpdate();

	smallEnemyActionPattern();//行動パターン更新

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