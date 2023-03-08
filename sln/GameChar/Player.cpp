#include "Player.h"
//#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "CharParameters.h"
#include "ParticleManager.h"

#include <DirectXMath.h>

using namespace DirectX;

//Player* Player::GetInstance()
//{
//	static Player instance;
//	return &instance;
//}

void Player::Attack()
{
	//キー入力使う
	Input* input = Input::GetInstance();

	CharParameters* charParameters = CharParameters::GetInstance();

	//triggerkey
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//パッド押した瞬間のみ
	const bool PadTriggerRB = input->TriggerButton(static_cast<int>(Button::RB));

	const bool TriggerMouseLEFT = input->TriggerMouse(0);

	//Input
	const bool InputSPACE = input->PushKey(DIK_SPACE);
	const bool PadInputRB = input->PushButton(static_cast<int>(Button::RB));
	const bool InputMouseLEFT = input->PushMouse(0);

	//弾速
	const int pBulVel = 60;

	//長押し発射
	if ((InputSPACE || PadInputRB || InputMouseLEFT) && AttackIntervalFlag == false)
	{
		XMFLOAT3 PlayerPos = obj->GetPosition();
		//弾生成
		std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
		//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
		madeBullet->Initialize();
		madeBullet->SetModel(pBulModel);
		madeBullet->SetPosition(PlayerPos);

		// velocityを算出 弾発射速度z
		DirectX::XMVECTOR vecvelocity = XMVectorSet(0, 0, pBulVel, 0);
		XMFLOAT3 xmfloat3velocity;
		XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

		madeBullet->SetVelocity(xmfloat3velocity);

		// 音声再生 鳴らしたいとき
		GameSound::GetInstance()->PlayWave("shot.wav", 0.1f);

		//弾登録
		bullets_.push_back(std::move(madeBullet));

		//input->PadVibrationDef();

		AttackIntervalFlag = true;
	}
	if (AttackIntervalFlag == true)
	{

		if (--AtkInterval_ >= 0) {//クールタイム 0まで減らす	

			//XMFLOAT3 PlayerPos = obj->GetPosition();
			////弾生成
			//std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
			////bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
			//madeBullet->Initialize();
			//madeBullet->SetModel(pBulModel);
			//madeBullet->SetPosition(PlayerPos);

			//// velocityを算出
			//XMVECTOR vecvelocity = XMVectorSet(0, 0, 3, 0);
			//XMFLOAT3 xmfloat3velocity;
			//XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

			//madeBullet->SetVelocity(xmfloat3velocity);

			//// 音声再生 鳴らしたいとき
			//GameSound::GetInstance()->PlayWave("shot.wav", 0.3f);

			////弾登録
			//bullets_.push_back(std::move(madeBullet));

			if (AtkInterval_ <= 0) {
				AttackIntervalFlag = false;
			}//0なったらくらい状態解除
		}
		else { AtkInterval_ = AtkInterval; }
	}

}

void Player::Move()
{

	Input* input = Input::GetInstance();
	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	//const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	//const bool inputC = input->PushKey(DIK_C);
	const bool inputI = input->PushKey(DIK_I);
	const bool inputJ = input->PushKey(DIK_J);
	const bool inputK = input->PushKey(DIK_K);
	const bool inputL = input->PushKey(DIK_L);
	const bool inputUp = input->PushKey(DIK_UP);
	const bool inputDown = input->PushKey(DIK_DOWN);
	const bool inputRight = input->PushKey(DIK_RIGHT);
	const bool inputLeft = input->PushKey(DIK_LEFT);
	//パッド押している間
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	const bool TriggerR = input->TriggerKey(DIK_R);

	XMFLOAT3 rotation = obj->GetRotation();

	//自分回転
	if (inputL)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.y++;
		obj->SetRotation(rotation);
	}
	if (inputJ)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.y--;
		obj->SetRotation(rotation);
	}
	if (inputI)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.x--;
		obj->SetRotation(rotation);
	}
	if (inputK)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.x++;
		obj->SetRotation(rotation);
	}
}

void Player::Shake() {

	CharParameters* charParameters = CharParameters::GetInstance();
	Input* input = Input::GetInstance();

	if (--pShakeTimer_ >= 0) {// 0まで減らす			
		//DebugText::GetInstance()->Print("Damage Cool Timev NOW", 200, 500, 4);

		input->PadVibration();

		//pos揺らす
		XMFLOAT3 pos = obj->GetPosition();
		randShakeNow = 1 + 1;//a~b
		pos.x = pos.x + rand() % randShakeNow - 0.5f;//a~bまでのrandShakeNowの最大値から半分を引いて負の数も含むように
		pos.y = pos.y + rand() % randShakeNow - 0.5f;
		obj->SetPosition(pos);

		if (pShakeTimer_ <= 0) {
			input->PadVibrationDef();
		}

	}
	else {
		pShakeTimer_ = pShakeTime;
		charParameters->SetispDam(false);
	}

}
void Player::PlayerDeath()
{
	Nowframe++;
	ParticleFrame++;
	PartTimeInterval = ParticleFrame / 40;

	XMFLOAT3 position = obj->GetPosition();

	if (GetPosFlag == true)
	{
		//最初の位置
		pPosDeath = obj->GetPosition();
		GetPosFlag = false;
	}

	//移動速度＝（指定座標-最初位置）/かかる時間
	MoveSp.x = (pPosDeath.x - pPosDeath.x) / NecesFrame;//ここの指定座標は自機最初のX座標にして真下に落ちるようにする
	MoveSp.y = (TargetPos.y - pPosDeath.y) / NecesFrame;
	MoveSp.z = (pPosDeath.z - pPosDeath.z) / NecesFrame;//奥行きついたらここもそうする
	//その時の位置＝最初位置＋移動速度＊経過時間
	NowPos.x = pPosDeath.x + MoveSp.x * Nowframe;
	NowPos.y = pPosDeath.y + MoveSp.y * Nowframe;
	NowPos.z = pPosDeath.z + MoveSp.z * Nowframe;

	obj->SetPosition(NowPos);//その時の位置

	//一定時間ごとにパーティクル
	if (PartTimeInterval == 1) {
		ExplosionFlag = true;
		// 音声再生 鳴らしたいとき
		GameSound::GetInstance()->PlayWave("destruction1.wav", 0.2f);
		ParticleManager::GetInstance()->CreateParticle(NowPos, 50, 30, 10);
		PartTimeInterval = 0;
		ParticleFrame = 0;
	}

	if (ExplosionFlag == true) {
		Shake();
		if (pShakeTimer_ <= 0) {
			ExplosionFlag = false;
		}
	}

	//回転
	XMFLOAT3 pRot = obj->GetRotation();
	pRot.z += pDeathRot;
	obj->SetRotation(pRot);

	if (obj->GetPosition().y < TargetPos.y)
	{
		//目標到達で死亡フラグオン
		PlayerDeathFlag = true;
	}
}

void Player::FiringLine()
{

	//いろいろ生成
	firingline_.reset(new PlayerFireLine());
	//いろいろキャラ初期化
	firingline_ = std::make_unique<PlayerFireLine>();
	firingline_->Initialize();
	firingline_->SetModel(pFiringLine);

	XMFLOAT3 PlayerPos = obj->GetPosition();
	firingline_->SetPosition({ PlayerPos.x,PlayerPos.y,PlayerPos.z });

	XMFLOAT3 PlayerRot = obj->GetRotation();
	firingline_->SetRotation(PlayerRot);

	//firingline_->SetScale({ 0.5f,0.5f,10.f });
}

void Player::Initialize()
{
	//定義とか仮おいておこう

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	//場所
	obj->SetPosition({ 0,70,-250 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("shot.wav");

}

void Player::Update()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	float pHp = charParameters->GetNowpHp();
	if (pHp <= 0) {
		isPHpLessThan0 = true;//自機体力が0を下回っている
	}

	//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
		});

	//攻撃してよい状況なら
	if (pAtkPossibleFlag) {
		//発射処理 生きててHp0以上なら
		if (alive && isPHpLessThan0 == false) {
			Attack();
		}
	}

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//生きててHp０いじょうなら
	if (alive && isPHpLessThan0 == false) {
		FiringLine();
		if (pAtkPossibleFlag) {//攻撃していいときなら
			firingline_->Update();
			Move();
		}
	}
	else {
		//0以下なったら
		PlayerDeath();
	}

	//自機が喰らってる状態になったら
	if (charParameters->GetispDam() == true) {
		//HP0以下ならやらないように　死亡演出やってるもんね
		if (charParameters->GetNowpHp() > 0)
		{
			Shake();
		}
	}

	//自機の弾威力がマイナス値にならない
	//0以下なら１にする
	if (pBulPower <= 0) {
		pBulPower = 1;
	}

	obj->Update();

}

void Player::Draw()
{
	CharParameters* charParameters = CharParameters::GetInstance();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive)
	{
		obj->Draw();
	}

	if (alive && isPHpLessThan0 == false) {
		if (pAtkPossibleFlag) {//攻撃していいときなら
			firingline_->Draw();
		}
	}
}
