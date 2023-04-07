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

	//移動完了確認しだい
	if (charParams->pNextPlaceGoFlag == false) {
		ActionStartPos = obj->GetPosition();//攻撃に移るときの座標取得Leaveで離れる限界値で使う
		actionPattern = std::bind(&Boss::Approach, this);
	}

	pos.y += 2.f * std::sin(time * 3.14159265358f);

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

	ApproachCount--;

	//ある程度近づいたら離れる
	if (ApproachCount == 0) {
		ApproachCount = ApproachCountDef;
		ChangeVerticalCount++;//縦攻撃するためのカウント進める
		actionPattern = std::bind(&Boss::Leave, this);
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
			if (even_odd_NumFlag)//奇数弾
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
		if (even_odd_NumFlag) { even_odd_NumFlag = false; }
		else { even_odd_NumFlag = true; }

		if (ChangeVerticalCount == ChangeVerticalNeces) {//縦攻撃カウントが一定に達したら
			actionPattern = std::bind(&Boss::Vertical, this);
		}
		else {
			actionPattern = std::bind(&Boss::Approach, this);
		}
	}
}
void Boss::Vertical()
{
	XMFLOAT3 position = obj->GetPosition();

	switch (verticalPattern_) {
	case VerticalPattern::def://デフォルト
		verticalPattern_ = VerticalPattern::StartVertical;//上昇開始
		break;

	case VerticalPattern::StartVertical://まずは上昇
		position.y += StartVerticalVal;
		StartVerticalVal++;

		if (position.y > DownStartPosY) {//一定超えたら判定切ってから待ち
			StartVerticalVal = StartVerticalValDef;//最初の上昇値戻す
			verticalPattern_ = VerticalPattern::Wait;//上昇開始
		}
		break;

	case VerticalPattern::Wait:
		doCollision = false;//Wait時は画面外なので当たり判定取らない
		VerticalWaitCount--;//待ちカウント減らす

		if (VerticalWaitCount == 0) {
			VerticalWaitCount = VerticalWaitCountDef / 2;//初回以降はデフォ/2にカウント戻す
			doCollision = true;//当たり判定オンにしてから次行動

			if (VerticalLoopCount != 0) {//UpDownループ回数が0でなければ、継続
				if (NextDown) {//次下降なら
					NextDown = false;//戻してから
					verticalPattern_ = VerticalPattern::Down;//下移動
				}
				if (NextUp) {//次上昇なら
					NextUp = false;//戻してから
					verticalPattern_ = VerticalPattern::Up;//上移動
				}
			}
			else {//るーぷが指定数おわったら（カウント0で）
				//次の動き戻す
				NextUp = false;
				NextDown = true;//最初は下降
				VerticalLoopCount = VerticalLoopCountDef;
				verticalPattern_ = VerticalPattern::Reverse;//戻るパターンへ
			}
		}
		break;

	case VerticalPattern::Down:
		if (VerticalStartPosFlag == false) {//最初に開始位置決める
			UpDownPos = { UpDownPos.x, DownStartPosY,position.z };//Yは開始位置　Zは元いた位置
			position = UpDownPos;//今の位置を下に下がる開始位置にする
			VerticalStartPosFlag = true;//最初の開始位置を決め終わった
		}
		position.y -= VerticalSp;//移動
		if (position.y <= UpStartPosY) {//上昇開始位置のYまできたら
			UpDownPos.x += NextMoveX;//次の移動時のためにX足しとく
			//↓待ち時間挟んでね　Wait時は判定切る
			VerticalStartPosFlag = false;//開始位置決定フラグ戻す
			NextUp = true;//次上昇
			VerticalLoopCount--;//Down抜けるときにデクリメント
			verticalPattern_ = VerticalPattern::Wait;//まってか
		}

		//発射カウントをデクリメント
		AtkCount--;
		//時が満ちたら
		if (AtkCount == 0) {
			//突撃時、生存時のみ発射
			if (alive) { Attack(); }//追尾弾
			//再びカウントできるように初期化
			AtkCount = Vertical_AtkInterval;
		}
		break;

	case VerticalPattern::Up:
		if (VerticalStartPosFlag == false) {//最初に開始位置決める
			UpDownPos = { UpDownPos.x, UpStartPosY,position.z };//Yは開始位置　Zは元いた位置
			position = UpDownPos;//今の位置を下に下がる開始位置にする
			VerticalStartPosFlag = true;//最初の開始位置を決め終わった
		}
		position.y += VerticalSp;//移動
		if (position.y >= DownStartPosY) {//上昇開始位置のYまできたら
			UpDownPos.x += NextMoveX;//次の移動時のためにX足しとく
			//↓待ち時間挟んでね　Wait時は判定切る
			VerticalStartPosFlag = false;//開始位置決定フラグ戻す
			NextDown = true;//次上昇
			verticalPattern_ = VerticalPattern::Wait;//まってから
		}

		AtkCount--;
		//時が満ちたら
		if (AtkCount == 0) {
			//突撃時、生存時のみ発射
			if (alive) { StraightAttack(); }
			//再びカウントできるように初期化
			AtkCount = Vertical_AtkInterval;
		}
		break;

	case VerticalPattern::Reverse:
		if (ReverseStartPosFlag == false) {//ここから戻り始める
			position = { ReverseStartPos.x,position.y,position.z };//X、Yは最後の高さのまま、Z
			ReverseStartPosFlag = true;
		}
		position.y += VerticalSp;

		if (position.y >= ReverseStartPos.y) {//ボス戦開始時のY座標到達で行動終わり
			ReverseStartPosFlag = false;
			verticalPattern_ = VerticalPattern::EndVertical;//縦パターンの行動終了する前にやること
		}
		break;

	case VerticalPattern::EndVertical:
		UpDownPos = UpDownPosDef;//上と下用の座標も元に戻す
		VerticalWaitCount = VerticalWaitCountDef;//待ちカウントデフォルトに戻す
		ChangeVerticalCount = ChangeVerticalCountDef;//縦攻撃するために必要なカウントをデフォに戻す
		verticalPattern_ = VerticalPattern::def;//また使えるようにデフォに戻しとく
		//大本の行動パターンをApproachに戻す
		actionPattern = std::bind(&Boss::Approach, this);
		break;
	}

	obj->SetPosition(position);
}

void Boss::HpHalfPatStart()
{
	CharParameters* charParams = CharParameters::GetInstance();

	Nowframe++;

	//自機の場所
	XMFLOAT3 pPos = shotTag->GetPosition();
	//指定座標で自機のZからどれくらい間をあけるか
	const int SpaceDistance = 500;

	if (GetPosOnlyFlag)
	{
		//最初の位置
		HpHalfMomentPos = obj->GetPosition();
		GetPosOnlyFlag = false;

		//指定座標どこか
		TargetHpHalfPos = { 0,0,pPos.z + SpaceDistance };

		//防御力上がる
		float Defence = charParams->GetBossDefense();
		Defence += 30;
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
			if (even_odd_NumFlag)//奇数弾
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
		Nowframe = NowframeDef;
		GetPosOnlyFlag = true;
		HpHalfMomentPos = {};
		MoveSp = {};
		NowPos = {};

		isHpHalfPattern = true;//この処理全部終了したからもうやんない
		actionPattern = std::bind(&Boss::CircularMotionMove, this);
	}
}
void Boss::CircularMotionMove()
{
	const float ApproachZ = 0.15f;//近づく値

	//敵の移動
	XMFLOAT3 position = obj->GetPosition();
	//弧度法
	HpHalf_rad = HpHalf_Angle * 3.1415926535f / 180.0f;

	//円の位置を三角関数でだす
	addX = cosf(HpHalf_rad) * HpHalf_Length;
	addY = sinf(HpHalf_rad) * HpHalf_Length;

	if (GetPosOnlyFlag)
	{
		//最初の位置
		CirclePosMem = obj->GetPosition();
		GetPosOnlyFlag = false;
	}

	XMFLOAT3 pPos = shotTag->GetPosition();
	//中心座標に移動量を足した値を座標に
	position.x = CirclePosMem.x + addX;
	position.y = CircularY + addY;
	position.z -= ApproachZ;//地味に迫ってくる

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
		GetPosOnlyFlag = true;
		actionPattern = std::bind(&Boss::LeaveFirstPos, this);
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

	if (GetPosOnlyFlag)
	{
		//最初の位置
		HpHalfMomentPos = obj->GetPosition();
		GetPosOnlyFlag = false;

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
		Nowframe = NowframeDef;
		GetPosOnlyFlag = true;
		HpHalfMomentPos = {};
		MoveSp = {};
		NowPos = {};

		//弾の発射間隔を元に戻す
		Circular_AtkInterval = Circular_AtkIntervalDef;

		if (PlungeCount == 0) {//LeaveFirstPosを指定回数したら突撃
			XMFLOAT3 boPos = obj->GetPosition();
			WasPosMem = boPos;//突っ込み行動へ移行する前に最後にいた場所を記憶する
			actionPattern = std::bind(&Boss::PlungeInto, this);
		}
		else {
			actionPattern = std::bind(&Boss::CircularMotionMove, this);
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

		Nowframe++;

		//移動速度＝（指定座標-最初位置）/かかる時間
		PlungeSp.x = (pPosMem.x - boPosMom.x);
		PlungeSp.y = (pPosMem.y - boPosMom.y);
		PlungeSp.z = (pPosMem.z - boPosMom.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&PlungeSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に(速度)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 50.f);
		// FLOAT3に変換
		XMStoreFloat3(&PlungeSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		PlungeNowPos.x = boPosMom.x + PlungeSp.x * Nowframe;
		PlungeNowPos.y = boPosMom.y + PlungeSp.y * Nowframe;
		PlungeNowPos.z = boPosMom.z + PlungeSp.z * Nowframe;

		obj->SetPosition(PlungeNowPos);//その時の位置

		if (position.z < shotTag->GetPosition().z) {//突撃終わったら
			boPosFlag = false;//一度きり読み込みリセ
			pMemFlag = false;//一度きりセット

			Nowframe = NowframeDef;
			//もう突っ込んだ
			PlungeCompletFlag = true;
			plungeIntoPattern_ = PlungeIntoPattern::Wait;//待ってから
		}
		break;

	case PlungeIntoPattern::Reverse://突っ込み終わったから戻れ

		ReversePos = { 0,100,shotTag->GetPosition().z + 1000 };

		Nowframe++;

		if (BeforeReversePosMemFlag == false) {
			BeforeReversePosMem = position;
			BeforeReversePosMemFlag = true;
		}

		//移動速度＝（指定座標-最初位置）/かかる時間
		ReverseSp.x = (ReversePos.x - BeforeReversePosMem.x);
		ReverseSp.y = (ReversePos.y - BeforeReversePosMem.y);
		ReverseSp.z = (ReversePos.z - BeforeReversePosMem.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecRevMoveSp = XMLoadFloat3(&ReverseSp);
		//normalize
		xmvecRevMoveSp = XMVector3Normalize(xmvecRevMoveSp);
		// 大きさを任意値に(速度)
		xmvecRevMoveSp = XMVectorScale(xmvecRevMoveSp, 10.f);
		// FLOAT3に変換
		XMStoreFloat3(&ReverseSp, xmvecRevMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		ReverseNowPos.x = BeforeReversePosMem.x + ReverseSp.x * Nowframe;
		ReverseNowPos.y = BeforeReversePosMem.y + ReverseSp.y * Nowframe;
		ReverseNowPos.z = BeforeReversePosMem.z + ReverseSp.z * Nowframe;

		obj->SetPosition(ReverseNowPos);//その時の位置

		//z座標が指定座標になったら
		if (position.z >= ReversePos.z) {
			PlungeCount = PlungeCountDef;
			BeforeReversePosMemFlag = false;
			plungeIntoPattern_ = PlungeIntoPattern::Leave;
			//突っ込み一連終わった後の行動へ
			actionPattern = std::bind(&Boss::AfterPlungeInto, this);
		}

		break;

	case PlungeIntoPattern::Wait://待機してから行動
		//---------------------予兆はここで シェイク
		//待機時間デクリメント
		PlungeIntoWaitCount--;

		//突撃後なら突っ込んだ場所でまつ
		if (PlungeCompletFlag) {
			obj->SetPosition(pPosMem);
		}
		else {
			XMFLOAT3 rot = obj->GetRotation();
			rot.z++;
			obj->SetRotation(rot);
		}

		if (PlungeIntoWaitCount == 0) {
			if (PlungeCompletFlag == false) {//突っ込み完了前なら
				//その時のターゲット座標
				//一度きり　突っ込みに使う座標
				if (pMemFlag == false) {
					pPosMem = shotTag->GetPosition();
					pMemFlag = true;
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
				PlungeCompletFlag = false;//リセット
				PlungeIntoWaitCount = PlungeIntoWaitCountDef;
				ShakePosMemFlag = false;//シェイク終わり
				Nowframe = NowframeDef;//戻さないと次の行動にカクツキが出る
				plungeIntoPattern_ = PlungeIntoPattern::Reverse;//元の場所へ向かう
			}
		}
		break;
	}
}

void Boss::AfterPlungeInto()
{
	switch (afterPlungePattern_)
	{
	case AfterPlungePattern::Wait://待機してから行動

		WaitTime--;//待ち時間
		if (WaitTime == 0) {//指定時間待ったら
			LoopCount++;//何回やったか数えるんだよ
			WaitTime = WaitTimeDef;
			Nowframe = NowframeDef;
			afterPlungePattern_ = AfterPlungePattern::Attack;//攻撃へ
		}

		AtkCount--;
		//時が満ちたら
		if (AtkCount == 0) {
			//突撃時、生存時のみ発射
			if (alive) { StraightAttack(); }
			//再びカウントできるように初期化
			AtkCount = AtkInterval;
		}

		//行動を一定数繰り返したら行動変える
		if (LoopCount == LoopCountMax) {
			Nowframe = 0;
			afterPlungePattern_ = AfterPlungePattern::Fin;//行動戻る前に最後に元の場所へ
			LoopCount = LoopCountDef;
		}

		break;

	case AfterPlungePattern::Attack:

		Nowframe++;

		if (pPosMemFlag == false) {
			pPosMem = shotTag->GetPosition();//自機いた場所
			boPosMem = obj->GetPosition();//ボスいた場所
			pPosMemFlag = true;
		}
		//移動速度＝（指定座標-最初位置）/かかる時間
		AtkMoveSp.x = (pPosMem.x - boPosMem.x) / NecesAtkMoveTime;
		AtkMoveSp.y = (pPosMem.y - boPosMem.y) / NecesAtkMoveTime;
		AtkMoveSp.z = (pPosMem.z + 800 - boPosMem.z) / NecesAtkMoveTime;

		//その時の位置＝最初位置＋移動速度＊経過時間
		boNowPos.x = boPosMem.x + AtkMoveSp.x * Nowframe;
		boNowPos.y = boPosMem.y + AtkMoveSp.y * Nowframe;
		boNowPos.z = boPosMem.z + AtkMoveSp.z * Nowframe;

		obj->SetPosition(boNowPos);//その時の位置

		AfterPlungePatAtkCount--;
		//時が満ちたら
		if (AfterPlungePatAtkCount == 0) {
			//突撃時、生存時のみ発射
			if (alive) { Attack(); }
			//再びカウントできるように初期化
			AfterPlungePatAtkCount = AfterPlungePatAtkInterval;
		}

		if (Nowframe == NecesAtkMoveTime) {
			Nowframe = NowframeDef;
			pPosMemFlag = false;
			afterPlungePattern_ = AfterPlungePattern::Wait;
		}

		break;

	case AfterPlungePattern::Fin:
		if (PlungeFinOnlyFlag == false) {//最初の座標
			boPosMem = obj->GetPosition();
			PlungeFinOnlyFlag = true;
		}
		else {
			float raito = (float)Nowframe / PlungeFinFrameMax;
			Nowframe++;

			//円行動に合うようにその場所へ移動
			XMFLOAT3 pos{};
			pos.x = std::lerp(boPosMem.x, obj->GetPosition().x, raito);
			pos.y = std::lerp(boPosMem.y, CircularY, raito);
			pos.z = std::lerp(boPosMem.z, obj->GetPosition().z, raito);
			obj->SetPosition(pos);
			if (Nowframe == PlungeFinFrameMax) {
				Nowframe = 0;
				PlungeFinOnlyFlag = false;
				afterPlungePattern_ = AfterPlungePattern::Wait;//ここの行動戻す
				//次の行動
				actionPattern = std::bind(&Boss::CircularMotionMove, this);
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
	if (ShakePosMemFlag) {
		pos.x = posMem.x + rand() % randShakeNow - 4.f;
		pos.y = posMem.y + rand() % randShakeNow - 4.f;
	}
	obj->SetPosition(pos);
}

//-------攻撃系
void Boss::Attack()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossAimBul> madeAimBullet = std::make_unique<BossAimBul>();
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
		if (aimBullet->ShotTagMomOnlyFlag) {
			aimBullet->ShotTagMoment = shotTag->GetPosition();
			aimBullet->ShotTagMomOnlyFlag = false;
		}

		aimBullet->Nowframe++;
		if (aimBullet->GetPosOnlyFlag)
		{
			//最初の位置
			aimBullet->boPosMoment = obj->GetPosition();
			aimBullet->GetPosOnlyFlag = false;
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
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 9.f);
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
	const float MoveZ = 1.7f;//移動量
	const float MoveX = 2.f;
	DirectX::XMVECTOR vecvelocity_center = DirectX::XMVectorSet(0, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_L = DirectX::XMVectorSet(-MoveX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_R = DirectX::XMVectorSet(MoveX, 0, MoveZ, 0);
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
	const float MoveZ = 1.7f;//移動量
	const float MoveEdgeX = 3.f;//端弾移動量
	const float MoveX = 0.5f;//移動量
	DirectX::XMVECTOR vecvelocity_L1 = DirectX::XMVectorSet(-MoveEdgeX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_L2 = DirectX::XMVectorSet(-MoveX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_R1 = DirectX::XMVectorSet(MoveX, 0, MoveZ, 0);
	DirectX::XMVECTOR vecvelocity_R2 = DirectX::XMVectorSet(MoveEdgeX, 0, MoveZ, 0);
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
void Boss::StraightAttack()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossStraightBul> madeBullet = std::make_unique<BossStraightBul>();
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet->Initialize();
	madeBullet->SetModel(StraightBulModel);
	madeBullet->SetPosition(position);
	madeBullet->SetScale({ 30.f, 30.f, 30.f });

	//弾登録
	straightBullets_.push_back(std::move(madeBullet));
}

//------攻撃系↑
void Boss::Death() {
	Nowframe++;
	ParticleFrame++;
	PartTimeInterval = ParticleFrame / 40;

	if (GetPosDeathOnlyFlag)
	{
		//最初の位置
		boPosDeath = obj->GetPosition();
		GetPosDeathOnlyFlag = false;
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
	mod_core.reset(Model::LoadFromOBJ("boss_core"));
	mod_AroundCore.reset(Model::LoadFromOBJ("boss_AroundCore"));
	mod_outside.reset(Model::LoadFromOBJ("boss_outside"));
	mod_SideSquare.reset(Model::LoadFromOBJ("boss_SideSquare"));
	mod_UpDown.reset(Model::LoadFromOBJ("boss_UpDown"));
	mod_VerticalCircle.reset(Model::LoadFromOBJ("boss_VerticalCircle"));
	//作る
	obj.reset(Object3d::Create());
	obj_core.reset(Object3d::Create());
	obj_AroundCore.reset(Object3d::Create());
	obj_outside.reset(Object3d::Create());
	obj_SideSquare.reset(Object3d::Create());
	obj_UpDown.reset(Object3d::Create());
	obj_VerticalCircle.reset(Object3d::Create());

	obj_core->SetModel(mod_core.get());
	obj_AroundCore->SetModel(mod_AroundCore.get());
	obj_outside->SetModel(mod_outside.get());
	obj_SideSquare->SetModel(mod_SideSquare.get());
	obj_UpDown->SetModel(mod_UpDown.get());
	obj_VerticalCircle->SetModel(mod_VerticalCircle.get());

	//-----↓任意↓-----//
		//大きさ
	obj->SetScale({ 60.0f, 60.0f, 60.0f });
	obj_core->SetScale({ 40.f, 40.f, 40.f });
	obj_AroundCore->SetScale({ 40.f, 40.f, 40.f });
	obj_outside->SetScale({ 40.f, 40.f, 40.f });
	obj_SideSquare->SetScale({ 40.f, 40.f, 40.f });
	obj_UpDown->SetScale({ 40.f, 40.f, 40.f });
	obj_VerticalCircle->SetScale({ 40.f, 40.f, 40.f });
	//場所
	obj->SetPosition({ 0,0,3300 });
	obj_core->SetPosition({ 0,30,2500 });
	obj_AroundCore->SetPosition({ 0,30,2500 });
	obj_outside->SetPosition({ 0,30,2500 });
	obj_SideSquare->SetPosition({ 0,30,2500 });
	obj_UpDown->SetPosition({ 0,30,2500 });
	obj_VerticalCircle->SetPosition({ 0,30,2500 });
	//回転
	obj_core->SetRotation({ 0, 180, 0 });
	obj_AroundCore->SetRotation({ 0, 180, 0 });
	obj_outside->SetRotation({ 0, 180, 0 });
	obj_SideSquare->SetRotation({ 0, 180, 0 });
	obj_UpDown->SetRotation({ 0, 180, 0 });
	obj_VerticalCircle->SetRotation({ 0, 180, 0 });

	obj->SetColor({ 1, 1, 1, 0.0f });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");
	GameSound::GetInstance()->LoadWave("destruction1.wav");

	//近づくパターン初期化
	ApproachInit();

	//デフォルトの行動を設定
	actionPattern = std::bind(&Boss::BossAppear, this);
}

void Boss::Update()
{
	Input* input = Input::GetInstance();
	CharParameters* charParameters = CharParameters::GetInstance();

	const bool input3 = input->PushKey(DIK_3);

	time = frame / 60.f;
	frame += 1.f;

	//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<BossBullet>& bullet) {
		return !bullet->GetAlive();
		});
	aimBullets_.remove_if([](std::unique_ptr<BossAimBul>& aimBullet) {
		return !aimBullet->GetAlive();
		});
	straightBullets_.remove_if([](std::unique_ptr<BossStraightBul>& straightBullet) {
		return !straightBullet->GetAlive();
		});

	if (isDeath) {
		actionPattern = std::bind(&Boss::Death, this);
		if (IsFirst_Death == false) {
			Nowframe = NowframeDef;
			IsFirst_Death = true;
		}
	}
	if (isHpHalfPattern == false) {
		if (charParameters->GetNowBoHp() <= charParameters->GetboMaxHp() / 2) {
			actionPattern = std::bind(&Boss::HpHalfPatStart, this);
		}
	}

	if (plungeIntoPattern_ == PlungeIntoPattern::Wait) {
		Shake();
	}

	//メンバ関数ポインタ呼び出し
	actionPattern();

	//弾更新
	for (std::unique_ptr<BossBullet>& bullet : bullets_) {
		bullet->Update();
	}
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {//狙い弾
		aimBullet->Update();
	}
	for (std::unique_ptr<BossStraightBul>& straightBullet : straightBullets_) {//狙い弾
		straightBullet->Update();
	}

	//狙い弾
	PAimBul();

	auto core_rot = obj_core->GetRotation();
	core_rot.y++;
	obj_core->SetRotation(core_rot);

	auto upDownRot = obj_UpDown->GetRotation();
	upDownRot.y++;
	obj_UpDown->SetRotation(upDownRot);

	auto VertCircleRot = obj_VerticalCircle->GetRotation();
	VertCircleRot.y--;
	obj_VerticalCircle->SetRotation(VertCircleRot);

	auto outSideRot = obj_outside->GetRotation();
	outSideRot.x++;
	obj_outside->SetRotation(outSideRot);

	auto sideSquareRot = obj_SideSquare->GetRotation();
	sideSquareRot.x--;
	obj_SideSquare->SetRotation(sideSquareRot);

	XMFLOAT3 pos = obj->GetPosition();
	obj_core->SetPosition(pos);
	obj_AroundCore->SetPosition(pos);
	obj_outside->SetPosition(pos);
	obj_SideSquare->SetPosition(pos);
	obj_UpDown->SetPosition(pos);
	obj_VerticalCircle->SetPosition(pos);

	obj->Update();
	obj_core->Update();
	obj_AroundCore->Update();
	obj_outside->Update();
	obj_SideSquare->Update();
	obj_UpDown->Update();
	obj_VerticalCircle->Update();
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
	for (std::unique_ptr<BossStraightBul>& straightBullet : straightBullets_) {//直線弾
		straightBullet->Draw();
	}

	if (alive) {
		//obj->Draw();
		obj_core->Draw();
		obj_AroundCore->Draw();
		obj_outside->Draw();
		obj_SideSquare->Draw();
		obj_UpDown->Draw();
		obj_VerticalCircle->Draw();
	}
}