#include "Boss.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "CharParameters.h"
#include "DxBase.h"
#include "GameUtility.h"

#include <DirectXMath.h>

#include "GamePlayScene.h"

using namespace DirectX;

void Boss::Initialize()
{
	particle.reset(new ParticleManager());
	particle->SetCamera(this->obj->GetCamera());

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
	obj->SetScale({ 130.0f, 130.0f, 130.0f });
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
	obj_core->SetColor({ coreCol });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("enemy_beam.wav");
	GameSound::GetInstance()->LoadWave("destruction1.wav");

	//近づくパターン初期化
	ApproachInit();

	//デフォルトの行動を設定
	actionPattern = std::bind(&Boss::BossAppear, this);

	plungeIntoPattern = std::bind(&Boss::PlungeIntoLeave, this);

	verticalPattern = std::bind(&Boss::StartVertical, this);

	afterPlungePattern = std::bind(&Boss::AfterPlungeWait, this);
}

void Boss::BossAppear()
{
	CharParameters* charParams = CharParameters::GetInstance();

	XMFLOAT3 pos = obj->GetPosition();

	//移動完了確認しだい
	if (charParams->pNextPlaceGoFlag == false) {
		actionStartPos = obj->GetPosition();//攻撃に移るときの座標取得Leaveで離れる限界値で使う
		actionPattern = std::bind(&Boss::Approach, this);
	}
	pos.y += 2.f * std::sinf(time * XM_PI);

	obj->SetPosition(pos);
}

void Boss::ApproachInit()
{
	//攻撃用カウント初期化して間隔代入すれば一旦待ってから発射可能
	atkCount = atkInterval;
	diffusionAtkCount = diffusionAtkInterval;
	circular_AtkCount = circular_AtkInterval;
}

void Boss::Approach()
{
	//---突撃---//
	//発射カウントをデクリメント
	atkCount--;
	//時が満ちたら
	if (atkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }
		//再びカウントできるように初期化
		atkCount = atkInterval;
	}

	//敵の移動
	XMFLOAT3 position = obj->GetPosition();
	position.z -= approachSpZ;
	position.y += approachSpY;
	position.x += 7.f * std::sinf(time * XM_PI);
	obj->SetPosition(position);

	approachCount--;

	//ある程度近づいたら離れる
	if (approachCount == 0) {
		approachCount = approachCountDef;
		changeVerticalCount++;//縦攻撃するためのカウント進める
		actionPattern = std::bind(&Boss::Leave, this);
	}
}

void Boss::Leave()
{
	//---後退---//

	//発射カウントをデクリメント
	diffusionAtkCount--;
	//時が満ちたら
	if (diffusionAtkCount == 0) {
		//生存時のみ発射
		if (alive) {
			if (even_odd_NumFlag)//奇数弾
			{
				DiffusionAttack();
			}
			else { DiffusionAttackEavenNumber(); }
		}
		//再びカウントできるように初期化
		diffusionAtkCount = diffusionAtkInterval;
	}

	//---後退---//
	XMFLOAT3 positionBack = obj->GetPosition();
	positionBack.z += leaveSpZ;
	positionBack.y -= leaveSpY;
	obj->SetPosition(positionBack);

	//離れる制限は自機の場所に自機と離したい距離分間を開ける
	const int SpaceDistance = 400;
	leaveLim = shotTag->GetPosition().z + SpaceDistance;

	//ある程度離れたら近づいてくる
	if (positionBack.z > actionStartPos.z && positionBack.y < actionStartPos.y) {
		if (even_odd_NumFlag) { even_odd_NumFlag = false; }
		else { even_odd_NumFlag = true; }

		if (changeVerticalCount == changeVerticalNeces) {//縦攻撃カウントが一定に達したら
			actionPattern = std::bind(&Boss::Vertical, this);
		}
		else {
			actionPattern = std::bind(&Boss::Approach, this);
		}
	}
}
void Boss::Vertical()
{
	//縦攻撃
	verticalPattern();
}
void Boss::StartVertical()
{
	XMFLOAT3 position = obj->GetPosition();
	//まずは上昇
	position.y += startVerticalVal;
	startVerticalVal++;

	if (position.y > downStartPosY) {//一定超えたら判定切ってから待ち
		startVerticalVal = startVerticalValDef;//最初の上昇値戻す
		verticalPattern = std::bind(&Boss::VerticalWait, this);
	}
	obj->SetPosition(position);
}
void Boss::VerticalWait()
{
	doCollision = false;//Wait時は画面外なので当たり判定取らない
	verticalWaitCount--;//待ちカウント減らす

	if (verticalWaitCount == 0) {
		verticalWaitCount = verticalWaitCountDef / 2;//初回以降はデフォ/2にカウント戻す
		doCollision = true;//当たり判定オンにしてから次行動

		if (verticalLoopCount != 0) {//UpDownループ回数が0でなければ、継続
			if (nextDown) {//次下降なら
				nextDown = false;//戻してから
				verticalPattern = std::bind(&Boss::VerticalDown, this);//下移動
			}
			if (nextUp) {//次上昇なら
				nextUp = false;//戻してから
				verticalPattern = std::bind(&Boss::VerticalUp, this);//上移動
			}
		}
		else {//るーぷが指定数おわったら（カウント0で）
			//次の動き戻す
			nextUp = false;
			nextDown = true;//最初は下降
			verticalLoopCount = verticalLoopCountDef;
			verticalPattern = std::bind(&Boss::VerticalReverse, this);//戻るパターンへ
		}
	}
}
void Boss::VerticalDown()
{
	XMFLOAT3 position = obj->GetPosition();
	if (verticalStartPosFlag == false) {//最初に開始位置決める
		upDownPos = { upDownPos.x, downStartPosY,position.z };//Yは開始位置　Zは元いた位置
		position = upDownPos;//今の位置を下に下がる開始位置にする
		verticalStartPosFlag = true;//最初の開始位置を決め終わった
	}
	position.y -= verticalSp;//移動
	if (position.y <= upStartPosY) {//上昇開始位置のYまできたら
		upDownPos.x += nextMoveX;//次の移動時のためにX足しとく
		//↓待ち時間挟んでね　Wait時は判定切る
		verticalStartPosFlag = false;//開始位置決定フラグ戻す
		nextUp = true;//次上昇
		verticalLoopCount--;//Down抜けるときにデクリメント
		verticalPattern = std::bind(&Boss::VerticalWait, this);//まってか
	}
	obj->SetPosition(position);

	//発射カウントをデクリメント
	atkCount--;
	//時が満ちたら
	if (atkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾
		//再びカウントできるように初期化
		atkCount = vertical_AtkInterval;
	}
}
void Boss::VerticalUp()
{
	XMFLOAT3 position = obj->GetPosition();
	if (verticalStartPosFlag == false) {//最初に開始位置決める
		upDownPos = { upDownPos.x, upStartPosY,position.z };//Yは開始位置　Zは元いた位置
		position = upDownPos;//今の位置を下に下がる開始位置にする
		verticalStartPosFlag = true;//最初の開始位置を決め終わった
	}
	position.y += verticalSp;//移動
	if (position.y >= downStartPosY) {//上昇開始位置のYまできたら
		upDownPos.x += nextMoveX;//次の移動時のためにX足しとく
		//↓待ち時間挟んでね　Wait時は判定切る
		verticalStartPosFlag = false;//開始位置決定フラグ戻す
		nextDown = true;//次上昇
		verticalPattern = std::bind(&Boss::VerticalWait, this);
	}
	obj->SetPosition(position);
	atkCount--;
	//時が満ちたら
	if (atkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { StraightAttack(); }
		//再びカウントできるように初期化
		atkCount = vertical_AtkInterval;
	}
}
void Boss::VerticalReverse()
{
	XMFLOAT3 position = obj->GetPosition();
	if (reverseStartPosFlag == false) {//ここから戻り始める
		position = { reverseStartPos.x,position.y,position.z };//X、Yは最後の高さのまま、Z
		reverseStartPosFlag = true;
	}
	position.y += verticalSp;
	obj->SetPosition(position);

	if (position.y >= reverseStartPos.y) {//ボス戦開始時のY座標到達で行動終わり
		reverseStartPosFlag = false;
		verticalPattern = std::bind(&Boss::EndVertical, this);//縦パターンの行動終了する前にやること
	}
}
void Boss::EndVertical()
{
	upDownPos = upDownPosDef;//上と下用の座標も元に戻す
	verticalWaitCount = verticalWaitCountDef;//待ちカウントデフォルトに戻す
	changeVerticalCount = changeVerticalCountDef;//縦攻撃するために必要なカウントをデフォに戻す
	verticalPattern = std::bind(&Boss::StartVertical, this);
	//大本の行動パターンをApproachに戻す
	actionPattern = std::bind(&Boss::Approach, this);
}

void Boss::HpHalfPatStart()
{
	CharParameters* charParams = CharParameters::GetInstance();

	nowframe++;

	//自機の場所
	XMFLOAT3 pPos = shotTag->GetPosition();
	//指定座標で自機のZからどれくらい間をあけるか
	const int SpaceDistance = 900;

	//コアの色を赤に
	constexpr float coreColChangeVal = 0.01f;//マイフレ変える値
	coreCol.x += coreColChangeVal;
	coreCol.z -= coreColChangeVal;
	obj_core->SetColor(coreCol);

	if (getPosOnlyFlag) {
		//最初の位置
		hpHalfMomentPos = obj->GetPosition();
		getPosOnlyFlag = false;

		//指定座標どこか
		targetHpHalfPos = { 0,0,pPos.z + SpaceDistance };

		//防御力上がる
		float Defence = charParams->GetBossDefense();
		Defence += 10;
		charParams->SetBossDefense(Defence);
	}

	float raito = (float)nowframe / necesHpHalfFrame;
	//場所移動
	obj->SetPosition(GameUtility::UtilLerp(hpHalfMomentPos, targetHpHalfPos, raito));

	//発射カウントをデクリメント
	diffusionAtkCount--;
	if (diffusionAtkCount == 0) {
		//生存時のみ発射
		if (alive) {
			if (even_odd_NumFlag)//奇数弾
			{
				DiffusionAttack();
			}
			else { DiffusionAttackEavenNumber(); }
		}
		//再びカウントできるように初期化
		diffusionAtkCount = diffusionAtkInterval;
	}
	//発射カウントをデクリメント
	atkCount--;
	//時が満ちたら
	if (atkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾
		//再びカウントできるように初期化
		atkCount = atkInterval;
	}

	if (nowframe == necesHpHalfFrame) {
		nowframe = nowframeDef;
		getPosOnlyFlag = true;
		hpHalfMomentPos = {};
		isHpHalfPattern = true;//この処理全部終了したからもうやんない
		actionPattern = std::bind(&Boss::CircularMotionMove, this);
	}
}
void Boss::CircularMotionMove()
{
	const float ApproachZ = 0.15f;//近づく値

	//弧度法
	hpHalf_rad = hpHalf_Angle * XM_PI / 180.0f;

	//円の位置を三角関数でだす
	addX = std::cosf(hpHalf_rad) * hpHalf_Length;
	addY = std::sinf(hpHalf_rad) * hpHalf_Length;

	XMFLOAT3 position = obj->GetPosition();
	if (getPosOnlyFlag)
	{
		//最初の位置
		circlePosMem = position;
		getPosOnlyFlag = false;
	}

	//敵の移動
	//中心座標に移動量を足した値を座標に
	obj->SetPosition({
		circlePosMem.x + addX ,
		circularY + addY,
		position.z - ApproachZ//地味に迫ってくる
		});

	hpHalf_Angle += 5.0f;//角度　増やすと移動速くなる
	hpHalf_Length += 0.3f;//渦を巻くように広げたい

	//半径が一定以上で行動変える
	if (hpHalf_Length >= 150) {
		//---円運動類
		hpHalf_Angle = hpHalf_AngleDef;
		hpHalf_rad = hpHalf_radDef;
		hpHalf_Length = hpHalf_LengthDef;
		addX = addXDef;
		addY = addYDef;
		getPosOnlyFlag = true;
		actionPattern = std::bind(&Boss::LeaveFirstPos, this);
	}

	//発射カウントをデクリメント
	circular_AtkCount--;
	//時が満ちたら
	if (circular_AtkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾

		//だんだん弾の発射間隔速く
		circular_AtkInterval -= 2;

		//再びカウントできるように初期化
		circular_AtkCount = circular_AtkInterval;
	}
}
void Boss::LeaveFirstPos()
{
	nowframe++;

	if (getPosOnlyFlag)
	{
		//最初の位置
		hpHalfMomentPos = obj->GetPosition();
		getPosOnlyFlag = false;

		//LeaveFirstPos2回で突っ込む
		plungeCount--;
	}

	bossLerpMoveRaito = (float)nowframe / necesLeaveFirstFrame;
	//場所移動
	//x自機ちょい追う感じ
	XMFLOAT3 endPos = targetHpHalfPos;
	endPos.x = shotTag->GetPosition().x - targetHpHalfPos.x;
	obj->SetPosition(GameUtility::UtilLerp(hpHalfMomentPos, endPos, bossLerpMoveRaito));

	//発射カウントをデクリメント
	atkCount--;
	//時が満ちたら
	if (atkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }//追尾弾
		//再びカウントできるように初期化
		atkCount = atkInterval_LeaveFirst;
	}

	if (nowframe == necesLeaveFirstFrame) {
		nowframe = nowframeDef;
		getPosOnlyFlag = true;
		hpHalfMomentPos = {};
		moveSp = {};

		//弾の発射間隔を元に戻す
		circular_AtkInterval = circular_AtkIntervalDef;

		if (plungeCount == 0) {//LeaveFirstPosを指定回数したら突撃
			XMFLOAT3 boPos = obj->GetPosition();
			wasPosMem = boPos;//突っ込み行動へ移行する前に最後にいた場所を記憶する
			actionPattern = std::bind(&Boss::PlungeInto, this);
		}
		else {
			actionPattern = std::bind(&Boss::CircularMotionMove, this);
		}
	}
}
void Boss::PlungeInto()
{
	//突っ込み行動パターン
	plungeIntoPattern();
}
void Boss::PlungeIntoLeave()
{
	XMFLOAT3 position = obj->GetPosition();
	//一度離れて
	position.z += leaveVel;

	if (position.z >= leavePos) {
		plungeIntoPattern = std::bind(&Boss::PlungeIntoWait, this);
	}

	obj->SetPosition(position);
}
void Boss::PlungeIntoWait()
{
	//待機してから行動
	//---------------------予兆はここで シェイク
	//待機時間デクリメント
	plungeIntoWaitCount--;

	//突撃後なら突っ込んだ場所でまつ
	if (plungeCompletFlag) {
		obj->SetPosition(pPosMem);
	}
	else {
		Shake();//揺らす
	}

	if (plungeIntoWaitCount == 0) {
		if (plungeCompletFlag == false) {//突っ込み完了前なら
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
			plungeIntoWaitCount = plungeIntoWaitCountDef;
			plungeIntoPattern = std::bind(&Boss::Plunge, this);
		}
		else {//突っ込み後
			plungeCompletFlag = false;//リセット
			plungeIntoWaitCount = plungeIntoWaitCountDef;
			shakePosMemFlag = false;//シェイク終わり
			nowframe = nowframeDef;//戻さないと次の行動にカクツキが出る
			plungeIntoPattern = std::bind(&Boss::PlungeIntoReverse, this);//元の場所へ向かう
		}
	}
}
void Boss::Plunge()
{
	//突っ込んでくる
	nowframe++;

	bossLerpMoveRaito = (float)nowframe / plungeNecesFrame;
	//場所移動
	obj->SetPosition(GameUtility::UtilLerp(boPosMom, pPosMem, bossLerpMoveRaito));

	XMFLOAT3 position = obj->GetPosition();
	if (position.z < shotTag->GetPosition().z) {//突撃終わったら
		boPosFlag = false;//一度きり読み込みリセ
		pMemFlag = false;//一度きりセット

		nowframe = nowframeDef;
		//もう突っ込んだ
		plungeCompletFlag = true;
		plungeIntoPattern = std::bind(&Boss::PlungeIntoWait, this);
	}
}
void Boss::PlungeIntoReverse()
{
	reverseTargetPos = { 0,100,shotTag->GetPosition().z + 1000 };

	nowframe++;
	XMFLOAT3 position = obj->GetPosition();
	if (beforeReversePosMemFlag == false) {
		beforeReversePosMem = position;
		beforeReversePosMemFlag = true;
	}

	bossLerpMoveRaito = (float)nowframe / plungeNecesFrame;
	//場所移動
	XMFLOAT3 reversePos{};
	reversePos = GameUtility::UtilLerp(beforeReversePosMem, reverseTargetPos, bossLerpMoveRaito);
	obj->SetPosition(reversePos);

	//z座標が指定座標になったら
	if (position.z >= reverseTargetPos.z) {
		plungeCount = plungeCountDef;
		beforeReversePosMemFlag = false;
		plungeIntoPattern = std::bind(&Boss::PlungeIntoLeave, this);
		//突っ込み一連終わった後の行動へ
		actionPattern = std::bind(&Boss::AfterPlungeInto, this);
	}
}

void Boss::AfterPlungeInto()
{
	afterPlungePattern();
}
void Boss::AfterPlungeWait()
{
	waitTime--;//待ち時間
	if (waitTime == 0) {//指定時間待ったら
		loopCount++;//何回やったか数えるんだよ
		waitTime = waitTimeDef;
		nowframe = nowframeDef;
		pPosMem = shotTag->GetPosition();//自機いた場所
		boPosMem = obj->GetPosition();//ボスいた場所
		afterPlungePattern = std::bind(&Boss::AfterPlungeAttack, this);//攻撃へ
	}

	atkCount--;
	//時が満ちたら
	if (atkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { StraightAttack(); }
		//再びカウントできるように初期化
		atkCount = atkInterval;
	}

	//行動を一定数繰り返したら行動変える
	if (loopCount == loopCountMax) {
		nowframe = 0;
		afterPlungePattern = std::bind(&Boss::AfterPlungeFin, this);//行動戻る前に最後に元の場所へ
		loopCount = loopCountDef;
	}
}
void Boss::AfterPlungeAttack()
{
	nowframe++;

	bossLerpMoveRaito = (float)nowframe / necesAtkMoveTime;
	//場所移動
	obj->SetPosition(GameUtility::UtilLerp(boPosMem, { pPosMem.x,pPosMem.y,boPosMem.z }, bossLerpMoveRaito));

	afterPlungePatAtkCount--;
	//時が満ちたら
	if (afterPlungePatAtkCount == 0) {
		//突撃時、生存時のみ発射
		if (alive) { Attack(); }
		//再びカウントできるように初期化
		afterPlungePatAtkCount = afterPlungePatAtkInterval;
	}

	if (nowframe == necesAtkMoveTime) {
		nowframe = nowframeDef;
		afterPlungePattern = std::bind(&Boss::AfterPlungeWait, this);
	}
}
void Boss::AfterPlungeFin()
{
	if (plungeFinOnlyFlag == false) {//最初の座標
		boPosMem = obj->GetPosition();
		plungeFinOnlyFlag = true;
	}
	else {
		bossLerpMoveRaito = (float)nowframe / plungeFinFrameMax;
		nowframe++;

		//円行動に合うようにその場所へ移動
		XMFLOAT3 endPos = obj->GetPosition();
		endPos.y = circularY;
		obj->SetPosition(GameUtility::UtilLerp(boPosMem, endPos, bossLerpMoveRaito));
		if (nowframe == plungeFinFrameMax) {
			nowframe = 0;
			plungeFinOnlyFlag = false;
			afterPlungePattern = std::bind(&Boss::AfterPlungeWait, this);//ここの行動戻す
			//次の行動
			actionPattern = std::bind(&Boss::CircularMotionMove, this);
		}
	}
}

void Boss::Shake() {
	CharParameters* charParameters = CharParameters::GetInstance();
	//pos揺らす
	XMFLOAT3 pos = obj->GetPosition();

	randShakeNow = 7 + 1;//a~b

	if (shakePosMemFlag == false) {
		posMem = pos;
		shakePosMemFlag = true;
	}
	if (shakePosMemFlag) {
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
	std::unique_ptr<BossAimBul>& madeAimBullet =
		aimBullets_.emplace_front(std::make_unique<BossAimBul>());
	madeAimBullet->Initialize();
	madeAimBullet->SetModel(aimBulModel);
	madeAimBullet->SetPosition(position);
}
void Boss::PAimBul()
{
	//弾一つずつ
	for (std::unique_ptr<BossAimBul>& aimBullet : aimBullets_) {
		//その時のターゲット座標
	//一度きり
		if (aimBullet->shotTagMomOnlyFlag) {
			aimBullet->shotTagMoment = shotTag->GetPosition();
			aimBullet->shotTagMomOnlyFlag = false;
		}

		aimBullet->nowframe++;
		if (aimBullet->getPosOnlyFlag)
		{
			//最初の位置
			aimBullet->boPosMoment = obj->GetPosition();
			aimBullet->getPosOnlyFlag = false;
		}
		//移動速度＝（指定座標-最初位置）/かかる時間
		aimBullet->moveSp.x = (aimBullet->shotTagMoment.x - aimBullet->boPosMoment.x);
		aimBullet->moveSp.y = (aimBullet->shotTagMoment.y - aimBullet->boPosMoment.y);
		aimBullet->moveSp.z = (aimBullet->shotTagMoment.z - aimBullet->boPosMoment.z);

		//XMVECTORに変換してxmvecMoveSpにいれる
		XMVECTOR xmvecMoveSp = XMLoadFloat3(&aimBullet->moveSp);
		//normalize
		xmvecMoveSp = XMVector3Normalize(xmvecMoveSp);
		// 大きさを任意値に(速度)
		xmvecMoveSp = XMVectorScale(xmvecMoveSp, 9.f);
		// FLOAT3に変換
		XMStoreFloat3(&aimBullet->moveSp, xmvecMoveSp);

		//その時の位置＝最初位置＋移動速度＊経過時間
		aimBullet->nowPos.x = aimBullet->boPosMoment.x + aimBullet->moveSp.x * aimBullet->nowframe;
		aimBullet->nowPos.y = aimBullet->boPosMoment.y + aimBullet->moveSp.y * aimBullet->nowframe;
		aimBullet->nowPos.z = aimBullet->boPosMoment.z + aimBullet->moveSp.z * aimBullet->nowframe;

		aimBullet->SetPosition(aimBullet->nowPos);//その時の位置
	}
}
void Boss::DiffusionAttack()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossBullet>& madeBullet_center =
		bullets_.emplace_front(std::make_unique<BossBullet>());
	std::unique_ptr<BossBullet>& madeBullet_L =
		bullets_.emplace_front(std::make_unique<BossBullet>());
	std::unique_ptr<BossBullet>& madeBullet_R =
		bullets_.emplace_front(std::make_unique<BossBullet>());

	madeBullet_center->Initialize();
	madeBullet_L->Initialize();
	madeBullet_R->Initialize();

	madeBullet_center->SetModel(bulModel);
	madeBullet_L->SetModel(bulModel);
	madeBullet_R->SetModel(bulModel);

	madeBullet_center->SetPosition(position);
	madeBullet_L->SetPosition(position);
	madeBullet_R->SetPosition(position);

	// velocityを算出
	const float MoveZ = 2.7f;//移動量
	const float MoveX = 3.f;
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
}
void Boss::DiffusionAttackEavenNumber()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossBullet>& madeBullet_L1 = bullets_.emplace_front(std::make_unique<BossBullet>());
	std::unique_ptr<BossBullet>& madeBullet_L2 = bullets_.emplace_front(std::make_unique<BossBullet>());
	std::unique_ptr<BossBullet>& madeBullet_R1 = bullets_.emplace_front(std::make_unique<BossBullet>());
	std::unique_ptr<BossBullet>& madeBullet_R2 = bullets_.emplace_front(std::make_unique<BossBullet>());
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet_L1->Initialize();
	madeBullet_L2->Initialize();
	madeBullet_R1->Initialize();
	madeBullet_R2->Initialize();

	madeBullet_L1->SetModel(bulModel);
	madeBullet_L2->SetModel(bulModel);
	madeBullet_R1->SetModel(bulModel);
	madeBullet_R2->SetModel(bulModel);

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
}

void Boss::StraightAttack()
{
	// 音声再生 鳴らしたいとき
	GameSound::GetInstance()->PlayWave("enemy_beam.wav", 0.3f);

	//弾発射
	XMFLOAT3 position = obj->GetPosition();
	//弾生成
	std::unique_ptr<BossStraightBul>& madeBullet =
		straightBullets_.emplace_front(std::make_unique<BossStraightBul>());
	//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
	madeBullet->Initialize();
	madeBullet->SetModel(straightBulModel);
	madeBullet->SetPosition(position);
	madeBullet->SetScale({ 30.f, 30.f, 30.f });
}

void Boss::DamageEffect()
{
	XMFLOAT4 col = { 1,reCol,reCol,1 };//赤
	reCol += reColVal;

	if (--bossBodyRedTime <= 0) {//時間になったら
		reCol = 0.f;//戻す
		col = { 1,1,1,1 };//本来の色
		bossBodyRedTime = bossBodyRedTimeDef;//カウント戻す
		bossDamageEffectFlag = false;//くらっていない状態に
	}
	obj->SetColor(col);
	//obj_core->SetColor(col);
	obj_AroundCore->SetColor(col);
	obj_outside->SetColor(col);
	obj_SideSquare->SetColor(col);
	obj_UpDown->SetColor(col);
	obj_VerticalCircle->SetColor(col);
}

//------攻撃系↑
void Boss::Death() {
	nowframe++;
	particleFrame++;
	partTimeInterval = particleFrame / 40;

	if (getPosDeathOnlyFlag) {
		//最初の位置
		boPosDeath = obj->GetPosition();
		getPosDeathOnlyFlag = false;
	}

	bossLerpMoveRaito = (float)nowframe / necesFrame;
	obj->SetPosition(GameUtility::UtilLerp(boPosDeath, { boPosDeath.x,targetPos.y,boPosDeath.z }, bossLerpMoveRaito));

	//一定時間ごとにパーティクル
	if (partTimeInterval == 1) {
		// 音声再生 鳴らしたいとき
		GameSound::GetInstance()->PlayWave("destruction1.wav", 0.2f);
		particle->CreateParticle(obj->GetPosition(), 100, 80, 10, { 1,0.1f,0.8f }, { 1,0,0 });
		partTimeInterval = 0;
		particleFrame = 0;
	}

	//コアの色変え 黒
	constexpr float coreColChangeNecesFrame = 60;//この時間かけて色変え

	XMFLOAT4 coreCol = obj_core->GetColor();
	coreColChangeRaito = (float)nowframe / coreColChangeNecesFrame;
	obj_core->SetColor({ std::lerp(1.f, 0.f, coreColChangeRaito),coreCol.y,coreCol.z,coreCol.w });

	XMFLOAT3 upDownRot = obj_UpDown->GetRotation();
	obj_UpDown->SetRotation({ upDownRot.x += 0.2f ,upDownRot.y,upDownRot.z });

	XMFLOAT3 verticalCircleDownRot = obj_VerticalCircle->GetRotation();
	obj_VerticalCircle->SetRotation({ verticalCircleDownRot.x += 0.2f ,verticalCircleDownRot.y,verticalCircleDownRot.z });

	XMFLOAT3 outSideRot = obj_outside->GetRotation();
	obj_outside->SetRotation({ outSideRot.x,outSideRot.y,outSideRot.z += 0.2f });

	XMFLOAT3 sideSquareRot = obj_SideSquare->GetRotation();
	obj_SideSquare->SetRotation({ sideSquareRot.x,sideSquareRot.y,sideSquareRot.z -= 0.2f });

	XMFLOAT3 aroundCoreRot = obj_AroundCore->GetRotation();
	obj_AroundCore->SetRotation({ aroundCoreRot.x -= 0.2f,aroundCoreRot.y,aroundCoreRot.z });
}

void Boss::AlwaysmMotion()
{
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
		if (isFirst_Death == false) {
			nowframe = nowframeDef;
			isFirst_Death = true;
		}
	}
	if (isHpHalfPattern == false) {
		if (charParameters->GetNowBoHp() <= charParameters->GetBoMaxHp() / 2) {
			actionPattern = std::bind(&Boss::HpHalfPatStart, this);
		}
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

	if (bossDamageEffectFlag) {//ダメージ演出フラグたったら
		DamageEffect();//体赤くする
	}

	if (!GetisDeath()) {
		AlwaysmMotion();//常に動かす
	}

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

	// パーティクル更新
	particle->Update();
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
		obj_core->Draw();
		obj_AroundCore->Draw();
		obj_outside->Draw();
		obj_SideSquare->Draw();
		obj_UpDown->Draw();
		obj_VerticalCircle->Draw();
	}
	DxBase* dxBase = DxBase::GetInstance();
	particle->Draw(dxBase->GetCmdList());
}