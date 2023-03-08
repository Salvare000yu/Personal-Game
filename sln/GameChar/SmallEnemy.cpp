#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include "GameSound.h"

#include <DirectXMath.h>
#include <random>

using namespace DirectX;

void SmallEnemy::Attack()
{
	//キー入力使う
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);

	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.1f);

	//弾発射
	XMFLOAT3 sePos = obj->GetPosition();
	//弾生成
	std::unique_ptr<SmallEnemyBullet> madeBullet = std::make_unique<SmallEnemyBullet>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet->Initialize();
	madeBullet->SetModel(seBulModel);
	madeBullet->SetPosition(sePos);

	//弾登録
	bullets_.push_back(std::move(madeBullet));
}

void SmallEnemy::Initialize()
{
	//std::srand((unsigned)std::time(nullptr));
	const float posXMin = -200.f;
	const float posXMax = 200.f;
	const float posYMin = 10.f;
	const float posYMax = 70.f;

	std::random_device seed;
	std::mt19937 random(seed());
	std::uniform_int_distribution<> posXNum(posXMin, posXMax);
	std::uniform_int_distribution<> posYNum(posYMin, posYMax);

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 6.0f, 6.0f, 6.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });
	//場所

	//SEneRandX = float(rand() % 60);
	//float SEneRandX = posXNum(random);
	//float SEneRandY = posYNum(random);
	//obj->SetPosition({ SEneRandX,SEneRandY,1000 });

	AtkCount = AtkInterval;

}

void SmallEnemy::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	//srand((unsigned int)timeGetTime());
	//SEneRandX = SEneMinX + (int)(rand() * (SEneMaxX - SEneMinX + 1) / (1 + RAND_MAX));
	//XMFLOAT3 position = obj->GetPosition();
	//position.x = SEneRandX;
	//obj->SetPosition(position);

	//if (input3) {
	//	XMFLOAT3 position = obj_enemy->GetPosition();
	//	position.z += 5;
	//	obj_enemy->SetPosition(position);
	//}
			//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<SmallEnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});

	XMFLOAT3 sePos = obj->GetPosition();
	//登場は奥から向かってくる
	if (isSeApproach == true) {
		if (obj->GetPosition().z > shotTag->GetPosition().z + PosZMax) {
			sePos.z -= 6;
		}
		else {
			RetireFrame--;//滞在時間
			if (RetireFrame < 0) {
				//捌け開始
				isRetire = true;
				isSeApproach = false;
			}
		}
	}
	
	if (isRetire == true) {
		//自機より右にいるか左にいるかでどちらに捌けるか変わる
		if (obj->GetPosition().x < shotTag->GetPosition().x) {
			retirePat_=RetirePat::Left;
			//左パターンへ
			isRetire = false;
		}
		else {
			retirePat_=RetirePat::Right;
			//右パターンへ
			isRetire = false;
		}
	}
	if (retirePat_ == RetirePat::Right) {
		sePos.x += 3;
	}
	if (retirePat_ == RetirePat::Left) {
		sePos.x -= 3;
	}

	obj->SetPosition(sePos);

	//発射カウントをデクリメント
	AtkCount--;

	//時間経過消滅
	if (--vanishTimer_ <= 0) { alive = false; }

	//時が満ちたら
	if (AtkCount == 0) {
		//生存時のみ発射
		if (alive) {
			Attack();
		}
		//再びカウントできるように初期化
		AtkCount = AtkInterval;
	}

	//弾更新
	for (std::unique_ptr<SmallEnemyBullet>& bullet : bullets_) {

		//その時のターゲット座標
		//一度きり
		if (bullet->ShotTagMomOnlyFlag == true) {
			bullet->ShotTagMoment = shotTag->GetPosition();
			bullet->ShotTagMomOnlyFlag = false;
		}

		bullet->Update();

		bullet->Nowframe++;
		if (bullet->GetPosOnlyFlag == true)
		{
			//最初の位置
			bullet->sePosMoment = obj->GetPosition();
			bullet->GetPosOnlyFlag = false;
		}
		//移動速度＝（指定座標-最初位置）/かかる時間
		// //絶対当たる
		////MoveSp.x = (shotTag->GetPosition().x - sePosMoment.x);
		////MoveSp.y = (shotTag->GetPosition().y - sePosMoment.y);
		////MoveSp.z = (shotTag->GetPosition().z - sePosMoment.z);
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
