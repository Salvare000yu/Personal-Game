#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "CharParameters.h"

#include <DirectXMath.h>

#include "GamePlayScene.h"

void Boss::BossAppear()
{

	CharParameters* charParams = CharParameters::GetInstance();

	XMFLOAT3 pos = obj->GetPosition();
	pos.z--;

	//移動完了確認しだい
	if (charParams->pNextPlaceGoFlag == false) {
		ActionStartPos = obj->GetPosition();//攻撃に移るときの座標取得Leaveで離れる限界値で使う
		actionPattern_ = ActionPattern::Approach;
	}

	pos.y += 2.f * sinf(time * 3.14159265358f);

	obj->SetPosition(pos);
}

void Boss::ApproachInit()
{
	//攻撃用カウント初期化して間隔代入すれば一旦待ってから発射可能
	AtkCount = AtkInterval;
	DiffusionAtkCount = DiffusionAtkInterval;
	Circular_AtkCount = Circular_AtkInterval;
}

void Boss::Approach()
{
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
	position.z -= ApproachSpZ;
	position.y += ApproachSpY;
	position.x += 7.f * sinf(time * 3.14159265358f);
	obj->SetPosition(position);

	//近づく制限は自機の場所に自機と離したい距離分間を開ける
	const int SpaceDistance = 200;
	ApproachLim = shotTag->GetPosition().z + SpaceDistance;

	//ある程度近づいたら離れる
	if (position.z < ApproachLim) {
		actionPattern_ = ActionPattern::Leave;
	}

}

void Boss::Leave()
{
	//---後退---//

	//発射カウントをデクリメント
	DiffusionAtkCount--;
	//時が満ちたら
	if (DiffusionAtkCount == 0) {
		//生存時のみ発射
		if (alive) {
			if (even_odd_NumFlag == true)//奇数弾
			{
				DiffusionAttack();
			}
			else { DiffusionAttackEavenNumber(); }
		}
		//再びカウントできるように初期化
		DiffusionAtkCount = DiffusionAtkInterval;
	}

	//---後退---//
	XMFLOAT3 positionBack = obj->GetPosition();
	positionBack.z += LeaveSpZ;
	positionBack.y -= LeaveSpY;
	obj->SetPosition(positionBack);

	//離れる制限は自機の場所に自機と離したい距離分間を開ける
	const int SpaceDistance = 400;
	LeaveLim = shotTag->GetPosition().z + SpaceDistance;

	//ある程度離れたら近づいてくる
	if (positionBack.z > ActionStartPos.z && positionBack.y < ActionStartPos.y) {
		if (even_odd_NumFlag == true) { even_odd_NumFlag = false; }
		else { even_odd_NumFlag = true; }
		actionPattern_ = ActionPattern::Approach;
	}
}

void Boss::HpHalfPatStart()
{

	CharParameters* charParams = CharParameters::GetInstance();

	Nowframe++;

	//自機の場所
	XMFLOAT3 pPos = shotTag->GetPosition();
	//指定座標で自機のZからどれくらい間をあけるか
	const int SpaceDistance = 250;

	if (GetPosFlag == true)
	{
		//最初の位置
		HpHalfMomentPos = obj->GetPosition();
		GetPosFlag = false;

		//指定座標どこか
		TargetHpHalfPos = { 0,0,pPos.z + SpaceDistance };

		//防御力上がる
		float Defence = charParams->GetBossDefense();
		Defence += 20;
		charParams->SetBossDefense(Defence);
	}

	//移動速度＝（指定座標-最初位置）/かかる時間
	MoveSp.x = (pPos.x - HpHalfMomentPos.x) / NecesHpHalfFrame;
	MoveSp.y = (TargetHpHalfPos.y - HpHalfMomentPos.y) / NecesHpHalfFrame;
	MoveSp.z = (TargetHpHalfPos.z - HpHalfMomentPos.z) / NecesHpHalfFrame;
	//その時の位置＝最初位置＋移動速度＊経過時間
	NowPos.x = HpHalfMomentPos.x + MoveSp.x * Nowframe;
	NowPos.y = HpHalfMomentPos.y + MoveSp.y * Nowframe;
	NowPos.z = HpHalfMomentPos.z + MoveSp.z * Nowframe;

	obj->SetPosition({ NowPos });

	//発射カウントをデクリメント
	DiffusionAtkCount--;
	if (DiffusionAtkCount == 0) {
		//生存時のみ発射
		if (alive) {
			if (even_odd_NumFlag == true)//奇数弾
			{
				DiffusionAttack();
			}
			else { DiffusionAttackEavenNumber(); }
		}
		//再びカウントできるように初期化
		DiffusionAtkCount = DiffusionAtkInterval;
	}
	//発射カウントをデクリメント
	AtkCount--;
	//時が満ちたら
	if (AtkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾
		//再びカウントできるように初期化
		AtkCount = AtkInterval;
	}

	if (Nowframe == NecesHpHalfFrame) {
		Nowframe = 0;
		GetPosFlag = true;
		HpHalfMomentPos = {};
		MoveSp = {};
		NowPos = {};

		isHpHalfPattern = true;//この処理全部終了したからもうやんない
		actionPattern_ = ActionPattern::CircularMotionMove;
	}
}
void Boss::CircularMotionMove()
{
	//敵の移動
	XMFLOAT3 position = obj->GetPosition();
	//弧度法
	HpHalf_rad = HpHalf_Angle * 3.1415926535f / 180.0f;

	//円の位置を三角関数でだす
	addX = cos(HpHalf_rad) * HpHalf_Length;
	addY = sin(HpHalf_rad) * HpHalf_Length;

	XMFLOAT3 pPos = shotTag->GetPosition();
	//中心座標に移動量を足した値を座標に
	position.x = pPos.x + addX;
	position.y = 40 + addY;
	position.z -= 0.15f;//地味に迫ってくる

	obj->SetPosition(position);

	HpHalf_Angle += 5.0f;//角度　増やすと移動速くなる
	HpHalf_Length += 0.3f;//渦を巻くように広げたい

	//半径が一定以上で行動変える
	if (HpHalf_Length >= 150) {
		//---円運動類
		HpHalf_Angle = HpHalf_AngleDef;
		HpHalf_rad = HpHalf_radDef;
		HpHalf_Length = HpHalf_LengthDef;
		addX = addXDef;
		addY = addYDef;

		actionPattern_ = ActionPattern::LeaveFirstPos;

	}

	//発射カウントをデクリメント
	Circular_AtkCount--;
	//時が満ちたら
	if (Circular_AtkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾

		//だんだん弾の発射間隔速く
		Circular_AtkInterval -= 2;

		//再びカウントできるように初期化
		Circular_AtkCount = Circular_AtkInterval;
	}
}
void Boss::LeaveFirstPos()
{

	Nowframe++;

	if (GetPosFlag == true)
	{
		//最初の位置
		HpHalfMomentPos = obj->GetPosition();
		GetPosFlag = false;

		//LeaveFirstPos2回で突っ込む
		PlungeCount--;

	}
	XMFLOAT3 pPos = shotTag->GetPosition();
	//移動速度＝（指定座標-最初位置）/かかる時間
	MoveSp.x = (pPos.x - HpHalfMomentPos.x) / NecesLeaveFirstFrame;
	MoveSp.y = (TargetHpHalfPos.y - HpHalfMomentPos.y) / NecesLeaveFirstFrame;
	MoveSp.z = (TargetHpHalfPos.z - HpHalfMomentPos.z) / NecesLeaveFirstFrame;
	//その時の位置＝最初位置＋移動速度＊経過時間
	NowPos.x = HpHalfMomentPos.x + MoveSp.x * Nowframe;
	NowPos.y = HpHalfMomentPos.y + MoveSp.y * Nowframe;
	NowPos.z = HpHalfMomentPos.z + MoveSp.z * Nowframe;

	obj->SetPosition({ NowPos });

	//発射カウントをデクリメント
	AtkCount--;
	//時が満ちたら
	if (AtkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾
		//再びカウントできるように初期化
		AtkCount = AtkInterval_LeaveFirst;
	}

	if (Nowframe == NecesLeaveFirstFrame) {
		Nowframe = 0;
		GetPosFlag = true;
		HpHalfMomentPos = {};
		MoveSp = {};
		NowPos = {};

		//弾の発射間隔を元に戻す
		Circular_AtkInterval = Circular_AtkIntervalDef;

		if (PlungeCount == 0) {//LeaveFirstPosを指定回数したら突撃
			XMFLOAT3 boPos = obj->GetPosition();
			WasPosMem = boPos;//突っ込み行動へ移行する前に最後にいた場所を記憶する
			actionPattern_ = ActionPattern::PlungeInto;
		}
		else {
			plungeIntoPattern_ = PlungeIntoPattern::Leave;
			actionPattern_ = ActionPattern::CircularMotionMove;
		}
	}
}
void Boss::PlungeInto()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	XMFLOAT3 position = obj->GetPosition();

	switch (plungeIntoPattern_)
	{

	case PlungeIntoPattern::Leave://一度離れて
		position.z += LeaveVel;

		if (position.z >= LeavePos) {
			plungeIntoPattern_ = PlungeIntoPattern::Wait;
		}

		obj->SetPosition(position);

		break;

	case PlungeIntoPattern::PlungeInto://突っ込んでくる
		//position.z -= PlungeVel;void Boss::PAimBul()

		PlungerFame++;

		//移動速度＝（指定座標-最初位置）/かかる時間
		PlungeSp.x = (pPosMom.x - boPosMom.x);
		PlungeSp.y = (pPosMom.y - boPosMom.y);
		PlungeSp.z = (pPosMom.z - boPosMom.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&PlungeSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に(速度)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 30.f);
		// FLOAT3に変換
		XMStoreFloat3(&PlungeSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		PlungeNowPos.x = boPosMom.x + PlungeSp.x * PlungerFame;
		PlungeNowPos.y = boPosMom.y + PlungeSp.y * PlungerFame;
		PlungeNowPos.z = boPosMom.z + PlungeSp.z * PlungerFame;

		obj->SetPosition(PlungeNowPos);//その時の位置

		{
			char tmp[32]{};
			sprintf_s(tmp, 32, "%2.f", PlungeNowPos.z);
			DebugText::GetInstance()->Print(tmp, 300, 390, 3);
		}
		{
			char tmp[32]{};
			sprintf_s(tmp, 32, "%2.f", PlungeNowPos.x);
			DebugText::GetInstance()->Print(tmp, 300, 430, 3);
		}

		if (position.z < charParameters->StopPos + 750) {//突撃終わったら

			boPosFlag = false;//一度きり読み込みリセ
			pMomFlag = false;//一度きりセット
			//もう突っ込んだ
			PlungeCompletFlag = true;
			plungeIntoPattern_ = PlungeIntoPattern::Wait;//待ってから
		}
		break;

	case PlungeIntoPattern::Reverse://突っ込み終わったから戻れ
		position.z += ReverseVel;
		if (position.z >= WasPosMem.z) {//突っ込み前の場所超えたら
			PlungeCount = PlungeCountDef;
			plungeIntoPattern_ = PlungeIntoPattern::Leave;
			actionPattern_ = ActionPattern::CircularMotionMove;//行動パターン戻す
		}
		obj->SetPosition(position);
		break;

	case PlungeIntoPattern::Wait://待機してから行動
		//---------------------予兆はここで シェイク
		//待機時間デクリメント
		PlungeIntoWaitCount--;

		if (PlungeIntoWaitCount == 0) {
			if (PlungeCompletFlag == false) {//突っ込み完了前なら
				//その時のターゲット座標
				//一度きり　突っ込みに使う座標
				if (pMomFlag == false) {
					pPosMom = shotTag->GetPosition();
					pMomFlag = true;
				}

				if (boPosFlag == false)
				{
					//最初の位置
					boPosMom = obj->GetPosition();
					boPosFlag = true;
				}
				PlungeIntoWaitCount = PlungeIntoWaitCountDef;
				plungeIntoPattern_ = PlungeIntoPattern::PlungeInto;
			}
			else {//突っ込み後
				obj->SetPosition(PlungeNowPos);//その時の位置
				PlungeCompletFlag = false;//リセット
				PlungerFame = 0;//経過時間リセット
				PlungeIntoWaitCount = PlungeIntoWaitCountDef;
				ShakePosMemFlag = false;//シェイク終わり
				plungeIntoPattern_ = PlungeIntoPattern::Reverse;//元の場所へ向かう
			}
		}
		break;
	}

}

void Boss::Shake() {

	CharParameters* charParameters = CharParameters::GetInstance();
	//pos揺らす
	XMFLOAT3 pos = obj->GetPosition();

	randShakeNow = 7 + 1;//a~b

	if (ShakePosMemFlag == false) {
		posMem = pos;
		ShakePosMemFlag = true;
	}
	if (ShakePosMemFlag == true) {
		pos.x = posMem.x + rand() % randShakeNow - 4.f;//a~bまでのrandShakeNowの最大値から半分を引いて負の数も含むように
		pos.y = posMem.y + rand() % randShakeNow - 4.f;
	}
	obj->SetPosition(pos);

}

//-------攻撃系
void Boss::Attack()
{
	//キー入力使う
	//Input* input = Input::GetInstance();

	//triggerkey
	//const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);

	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossAimBul> madeAimBullet = std::make_unique<BossAimBul>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeAimBullet->Initialize();
	madeAimBullet->SetModel(AimBulModel);
	madeAimBullet->SetPosition(position);

	//弾登録
	aimBullets_.push_back(std::move(madeAimBullet));
}
void Boss::PAimBul()
{
	//弾一つずつ
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {
		//その時のターゲット座標
	//一度きり
		if (aimBullet->ShotTagMomFlag == true) {
			aimBullet->ShotTagMoment = shotTag->GetPosition();
			aimBullet->ShotTagMomFlag = false;
		}

		aimBullet->Nowframe++;
		if (aimBullet->GetPosFlag == true)
		{
			//最初の位置
			aimBullet->boPosMoment = obj->GetPosition();
			aimBullet->GetPosFlag = false;
		}
		//移動速度＝（指定座標-最初位置）/かかる時間
		// //絶対当たる
		////MoveSp.x = (shotTag->GetPosition().x - sePosMoment.x);
		////MoveSp.y = (shotTag->GetPosition().y - sePosMoment.y);
		////MoveSp.z = (shotTag->GetPosition().z - sePosMoment.z);
		aimBullet->MoveSp.x = (aimBullet->ShotTagMoment.x - aimBullet->boPosMoment.x);
		aimBullet->MoveSp.y = (aimBullet->ShotTagMoment.y - aimBullet->boPosMoment.y);
		aimBullet->MoveSp.z = (aimBullet->ShotTagMoment.z - aimBullet->boPosMoment.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&aimBullet->MoveSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に(速度)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 7.f);
		// FLOAT3に変換
		XMStoreFloat3(&aimBullet->MoveSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		aimBullet->NowPos.x = aimBullet->boPosMoment.x + aimBullet->MoveSp.x * aimBullet->Nowframe;
		aimBullet->NowPos.y = aimBullet->boPosMoment.y + aimBullet->MoveSp.y * aimBullet->Nowframe;
		aimBullet->NowPos.z = aimBullet->boPosMoment.z + aimBullet->MoveSp.z * aimBullet->Nowframe;

		aimBullet->SetPosition(aimBullet->NowPos);//その時の位置
	}
}
void Boss::DiffusionAttack()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossBullet> madeBullet_center = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_L = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_R = std::make_unique<BossBullet>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet_center->Initialize();
	madeBullet_L->Initialize();
	madeBullet_R->Initialize();

	madeBullet_center->SetModel(BulModel);
	madeBullet_L->SetModel(BulModel);
	madeBullet_R->SetModel(BulModel);

	madeBullet_center->SetPosition(position);
	madeBullet_L->SetPosition(position);
	madeBullet_R->SetPosition(position);

	// velocityを算出
	DirectX::XMVECTOR vecvelocity_center = DirectX::XMVectorSet(0, 0, 1.2, 0);
	DirectX::XMVECTOR vecvelocity_L = DirectX::XMVectorSet(-2, 0, 1, 0);
	DirectX::XMVECTOR vecvelocity_R = DirectX::XMVectorSet(2, 0, 1, 0);
	XMFLOAT3 xmfloat3velocity_center;
	XMFLOAT3 xmfloat3velocity_L;
	XMFLOAT3 xmfloat3velocity_R;
	XMStoreFloat3(&xmfloat3velocity_center, XMVector3Transform(vecvelocity_center, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_L, XMVector3Transform(vecvelocity_L, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_R, XMVector3Transform(vecvelocity_R, obj->GetMatRot()));

	madeBullet_center->SetVelocity(xmfloat3velocity_center);
	madeBullet_L->SetVelocity(xmfloat3velocity_L);
	madeBullet_R->SetVelocity(xmfloat3velocity_R);

	//弾登録
	bullets_.push_back(std::move(madeBullet_center));
	bullets_.push_back(std::move(madeBullet_L));
	bullets_.push_back(std::move(madeBullet_R));
}
void Boss::DiffusionAttackEavenNumber()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossBullet> madeBullet_L1 = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_L2 = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_R1 = std::make_unique<BossBullet>();
	std::unique_ptr<BossBullet> madeBullet_R2 = std::make_unique<BossBullet>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet_L1->Initialize();
	madeBullet_L2->Initialize();
	madeBullet_R1->Initialize();
	madeBullet_R2->Initialize();

	madeBullet_L1->SetModel(BulModel);
	madeBullet_L2->SetModel(BulModel);
	madeBullet_R1->SetModel(BulModel);
	madeBullet_R2->SetModel(BulModel);

	madeBullet_L1->SetPosition(position);
	madeBullet_L2->SetPosition(position);
	madeBullet_R1->SetPosition(position);
	madeBullet_R2->SetPosition(position);

	// velocityを算出
	DirectX::XMVECTOR vecvelocity_L1 = DirectX::XMVectorSet(-3, 0, 1, 0);
	DirectX::XMVECTOR vecvelocity_L2 = DirectX::XMVectorSet(-0.5, 0, 1, 0);
	DirectX::XMVECTOR vecvelocity_R1 = DirectX::XMVectorSet(0.5, 0, 1, 0);
	DirectX::XMVECTOR vecvelocity_R2 = DirectX::XMVectorSet(3, 0, 1, 0);
	XMFLOAT3 xmfloat3velocity_L1;
	XMFLOAT3 xmfloat3velocity_L2;
	XMFLOAT3 xmfloat3velocity_R1;
	XMFLOAT3 xmfloat3velocity_R2;
	XMStoreFloat3(&xmfloat3velocity_L1, XMVector3Transform(vecvelocity_L1, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_L2, XMVector3Transform(vecvelocity_L2, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_R1, XMVector3Transform(vecvelocity_R1, obj->GetMatRot()));
	XMStoreFloat3(&xmfloat3velocity_R2, XMVector3Transform(vecvelocity_R2, obj->GetMatRot()));

	madeBullet_L1->SetVelocity(xmfloat3velocity_L1);
	madeBullet_L2->SetVelocity(xmfloat3velocity_L2);
	madeBullet_R1->SetVelocity(xmfloat3velocity_R1);
	madeBullet_R2->SetVelocity(xmfloat3velocity_R2);

	//弾登録
	bullets_.push_back(std::move(madeBullet_L1));
	bullets_.push_back(std::move(madeBullet_L2));
	bullets_.push_back(std::move(madeBullet_R1));
	bullets_.push_back(std::move(madeBullet_R2));
}
//------攻撃系↑
void Boss::Death() {

	Nowframe++;
	ParticleFrame++;
	PartTimeInterval = ParticleFrame / 40;

	if (GetPosDeathFlag == true)
	{
		//最初の位置
		boPosDeath = obj->GetPosition();
		GetPosDeathFlag = false;
	}

	//XMFLOAT3 boRot = obj->GetRotation();
	//移動速度＝（指定座標-最初位置）/かかる時間
	MoveSp.x = (boPosDeath.x - boPosDeath.x) / NecesFrame;//指定座標Xは死んだ場所から真下に落ちるように死んだX座標
	MoveSp.y = (TargetPos.y - boPosDeath.y) / NecesFrame;
	MoveSp.z = (boPosDeath.z - boPosDeath.z) / NecesFrame;//〃
	//その時の位置＝最初位置＋移動速度＊経過時間
	NowPos.x = boPosDeath.x + MoveSp.x * Nowframe;
	NowPos.y = boPosDeath.y + MoveSp.y * Nowframe;
	NowPos.z = boPosDeath.z + MoveSp.z * Nowframe;

	//boRot.y += +1.0;
	//boPos.y += -0.5;
	//ParticleManager::GetInstance()->CreateParticle(boPos, 100, 50, 5);
	obj->SetPosition(NowPos);//その時の位置

	//一定時間ごとにパーティクル
	if (PartTimeInterval == 1) {
		// 音声再生 鳴らしたいとき
		GameSound::GetInstance()->PlayWave("destruction1.wav", 0.2f);
		ParticleManager::GetInstance()->CreateParticle(NowPos, 100, 80, 10);
		PartTimeInterval = 0;
		ParticleFrame = 0;
	}
	//obj->SetRotation(boRot);
}

void Boss::Initialize()
{

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 50.0f, 50.0f, 50.0f });
	//場所
	obj->SetPosition({ 0,50,3300 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");
	GameSound::GetInstance()->LoadWave("destruction1.wav");

	//近づくパターン初期化
	ApproachInit();
}

void Boss::Update()
{
	Input* input = Input::GetInstance();
	CharParameters* charParameters = CharParameters::GetInstance();

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
	aimBullets_.remove_if([](std::unique_ptr<BossAimBul>& aimBullet) {
		return !aimBullet->GetAlive();
		});

	//黄金の回転
	for (int i = 0; i < 1; i++)
	{
		frame += 1.f;

		//XMFLOAT3 rotation = obj->GetRotation();
		//rotation.y += 0.7f;
		//rotation.x += 0.4f;
		//obj->SetRotation({ rotation });

		//XMFLOAT3 position = obj->GetPosition();
		//position.x += 5.f * sin(time * 3.14159265358f);
		//obj->SetPosition(position);

	}

	//メンバ関数ポインタ対応したボスの動きをする
	if (actionPattern_ == ActionPattern::BossAppear) { pFunc = &Boss::BossAppear; }
	if (actionPattern_ == ActionPattern::Approach) { pFunc = &Boss::Approach; }
	if (actionPattern_ == ActionPattern::Leave) { pFunc = &Boss::Leave; }
	if (actionPattern_ == ActionPattern::HpHalfPatStart) { pFunc = &Boss::HpHalfPatStart; }
	if (actionPattern_ == ActionPattern::Death) { pFunc = &Boss::Death; }
	if (actionPattern_ == ActionPattern::CircularMotionMove) { pFunc = &Boss::CircularMotionMove; }
	if (actionPattern_ == ActionPattern::LeaveFirstPos) { pFunc = &Boss::LeaveFirstPos; }
	if (actionPattern_ == ActionPattern::PlungeInto) { pFunc = &Boss::PlungeInto; }

	if (isDeath == true) {
		actionPattern_ = ActionPattern::Death;
		if (IsFirst_Death == false) {
			Nowframe = 0;
			IsFirst_Death = true;
		}
	}
	if (isHpHalfPattern == false) {
		if (charParameters->GetNowBoHp() <= charParameters->GetboMaxHp() / 2) {
			actionPattern_ = ActionPattern::HpHalfPatStart;
		}
	}

	if (plungeIntoPattern_ == PlungeIntoPattern::Wait) {
		Shake();
	}

	//メンバ関数ポインタ呼び出し
	(this->*pFunc)();

	//弾更新
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {//狙い弾
		aimBullet->Update();
	}

	//狙い弾
	PAimBul();

	obj->Update();

	//{
	//	char tmp[32]{};
	//	sprintf_s(tmp, 32, "BDeathFrame : %2.f", Nowframe);
	//	DebugText::GetInstance()->Print(tmp, 150, 290, 3);
	//}
}

void Boss::Draw()
{
	//弾
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Draw();
	}
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {//狙い弾
		aimBullet->Draw();
	}

	if (alive) {
		obj->Draw();
	}
}
