#include "Player.h"
//#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "CharParameters.h"
#include "DebugText.h"
#include "DxBase.h"

#include <DirectXMath.h>

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min

using namespace DirectX;

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
	if ((InputSPACE || PadInputRB || InputMouseLEFT) && attackIntervalFlag == false)
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

		attackIntervalFlag = true;
	}
	if (attackIntervalFlag)
	{
		if (--atkInterval_ >= 0) {//クールタイム 0まで減らす
			if (atkInterval_ <= 0) {
				attackIntervalFlag = false;
			}//0なったらくらい状態解除
		}
		else { atkInterval_ = atkInterval; }
	}
}

void Player::Move()
{
	Input* input = Input::GetInstance();
	const bool inputI = input->PushKey(DIK_I);
	const bool inputJ = input->PushKey(DIK_J);
	const bool inputK = input->PushKey(DIK_K);
	const bool inputL = input->PushKey(DIK_L);
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
	nowframe++;
	particleFrame++;
	partTimeInterval = particleFrame / 40;

	XMFLOAT3 position = obj->GetPosition();

	if (getPosFlag)
	{
		//最初の位置
		pPosDeath = obj->GetPosition();
		getPosFlag = false;
	}

	//移動速度＝（指定座標-最初位置）/かかる時間
	moveSp.x = (pPosDeath.x - pPosDeath.x) / necesFrame;//ここの指定座標は自機最初のX座標にして真下に落ちるようにする
	moveSp.y = (targetPos.y - pPosDeath.y) / necesFrame;
	moveSp.z = (pPosDeath.z - pPosDeath.z) / necesFrame;//奥行きついたらここもそうする
	//その時の位置＝最初位置＋移動速度＊経過時間
	nowPos.x = pPosDeath.x + moveSp.x * nowframe;
	nowPos.y = pPosDeath.y + moveSp.y * nowframe;
	nowPos.z = pPosDeath.z + moveSp.z * nowframe;

	obj->SetPosition(nowPos);//その時の位置

	//一定時間ごとにパーティクル
	if (partTimeInterval == 1) {
		explosionFlag = true;
		// 音声再生 鳴らしたいとき
		GameSound::GetInstance()->PlayWave("destruction1.wav", 0.2f);
		particle->CreateParticle(nowPos, 50, 30, 10, { 1,0.1f,0.8f }, { 1,0,0 });
		partTimeInterval = 0;
		particleFrame = 0;
	}

	if (explosionFlag) {
		Shake();
		if (pShakeTimer_ <= 0) {
			explosionFlag = false;
		}
	}

	//回転
	XMFLOAT3 pRot = obj->GetRotation();
	pRot.z += pDeathRot;
	obj->SetRotation(pRot);

	if (obj->GetPosition().y < targetPos.y)
	{
		//目標到達で死亡フラグオン
		playerDeathFlag = true;
	}
}

void Player::Initialize()
{
	particle.reset(new ParticleManager());
	particle->SetCamera(this->obj->GetCamera());

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	//場所
	obj->SetPosition({ 0,70,-250 });

	mod_firingline.reset(Model::LoadFromOBJ("firing_line"));
	//いろいろ生成
	firingline_.reset(new PlayerFireLine());
	//いろいろキャラ初期化
	firingline_ = std::make_unique<PlayerFireLine>();
	firingline_->Initialize();
	firingline_->SetModel(mod_firingline.get());
	firingline_->GetObj()->SetParent(this->GetObj());
	firingline_->SetScale({ 0.2f,0.2f,1000 });
	firingline_->SetAlive(false);

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
		particle->Add(30,
			bullet->GetPosition(),
			{ 0,0,0 },
			{ 0,0,0 },
			100.f, 0.f,
			{ 0,0.75f,0.75f }, { 1,0,1 });
	}

	//生きててHp０いじょうなら
	if (alive && isPHpLessThan0 == false) {
		if (pAtkPossibleFlag) {//攻撃していいときなら
			Move();
		}
	}
	else {
		//0以下なったら攻撃不可フラグたてて死亡演出
		pAtkPossibleFlag = false;
		PlayerDeath();
	}

	//自機が喰らってる状態になったら
	if (charParameters->GetispDam()) {
		//HP0以下ならやらないように　死亡演出やってるもんね
		if (charParameters->GetNowpHp() > 0)
		{
			Shake();
		}
	}

	//自機弾威力最小値１
	pBulPower = std::max(pBulPower, 1.f);

	// パーティクル更新
	particle->Update();

	obj->Update();
	firingline_->Update();//射線
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
		firingline_->Draw();//射線
	}
	DxBase* dxBase = DxBase::GetInstance();
	particle->Draw(dxBase->GetCmdList());
}