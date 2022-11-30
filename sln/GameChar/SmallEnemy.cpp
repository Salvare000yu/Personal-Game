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
	float SEneRandX = posXNum(random);
	float SEneRandY = posYNum(random);
	obj->SetPosition({ SEneRandX,SEneRandY,1000 });

	AtkCount = AtkInterval;

	Nowframe = 0;//現在フレ
	GetPosFlag = true;//一度きり座標読み取りフラグ
	NowPos={};//その時の弾位置
	sePosMoment={};//発射時の雑魚敵位置
	MoveSp={};//弾移動速度
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

	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 smEnemPos = obj->GetPosition();
		smEnemPos.z -= 6;
		obj->SetPosition(smEnemPos);

		//発射カウントをデクリメント
		AtkCount--;
	}

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

		bullet->Update();

		Nowframe++;
		if (GetPosFlag == true)
		{
			//最初の位置
			sePosMoment = obj->GetPosition();
			GetPosFlag = false;
		}
		//移動速度＝（指定座標-最初位置）/かかる時間
		MoveSp.x = (0 - sePosMoment.x);
		MoveSp.y = (40 - sePosMoment.y);
		MoveSp.z = (-170 - sePosMoment.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&MoveSp);
		//normalize
		xmvecMoveSp= XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 10.f);
		// FLOAT3に変換
		XMStoreFloat3(&MoveSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		NowPos.x = sePosMoment.x + MoveSp.x * Nowframe;
		NowPos.y = sePosMoment.y + MoveSp.y * Nowframe;
		NowPos.z = sePosMoment.z + MoveSp.z * Nowframe;

		bullet->SetPosition(NowPos);//その時の位置
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
