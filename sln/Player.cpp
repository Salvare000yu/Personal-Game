#include "Player.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

void Player::Attack()
{
	//キー入力使う
	Input* input = Input::GetInstance();

	//triggerkey
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//パッド押した瞬間のみ
	const bool PadTriggerRB = input->TriggerButton(static_cast<int>(Button::RB));

	//弾発射
	if (TriggerSPACE|| PadTriggerRB) {
		XMFLOAT3 PlayerPos = obj_player->GetPosition();
		//弾生成
		std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
		//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
		madeBullet->Initialize({ PlayerPos });

		//弾登録
		bullets_.push_back(std::move(madeBullet));
	}
}

void Player::Initialize()
{
	//定義とか仮おいておこう


	//もでる読み込み
	mod_player.reset(Model::LoadFromOBJ("hiyoko"));
	//作る
	obj_player.reset(Object3d::Create());
	//セット
	obj_player->SetModel(mod_player.get());
	//-----↓任意↓-----//
	//大きさ
	obj_player->SetScale({ 3.0f, 3.0f, 3.0f });
	//場所
	obj_player->SetPosition({ 0,40,-170 });
}

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

DirectX::XMFLOAT3 Player::GetPlayerPosMemory()
{
	XMFLOAT3 PlayerPosMemory = {};
	return PlayerPosMemory;
}

void Player::OnCollision()
{
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	//パッド押している間
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	const bool TriggerR = input->TriggerKey(DIK_R);

	time = frame / 60.f;

	//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsVanish();
		});

	//----------↓移動制限 カメラと同じにする
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 100;//下に行ける範囲
	const float PlayerMinMoveLimY = 200;//上に行ける範囲

	const float PlayerMaxMoveLimZ = 290;//後ろ
	const float PlayerMinMoveLimZ = 200;

	XMFLOAT3 PlayerPos = obj_player->GetPosition();
	PlayerPos.x = max(PlayerPos.x, -PlayerMoveLimX);
	PlayerPos.x = min(PlayerPos.x, +PlayerMoveLimX);
	PlayerPos.y = max(PlayerPos.y, -PlayerMaxMoveLimY);//下に行ける範囲
	PlayerPos.y = min(PlayerPos.y, +PlayerMinMoveLimY);//上に行ける範囲
	PlayerPos.z = max(PlayerPos.z, -PlayerMaxMoveLimZ);
	PlayerPos.z = min(PlayerPos.z, +PlayerMinMoveLimZ);
	obj_player->SetPosition(PlayerPos);
	//----------↑移動制限

	//------------------↓プレイヤー移動＆姿勢
	if (inputW || inputS || inputA || inputD || inputQ || inputZ|| PadInputUP|| PadInputDOWN|| PadInputLEFT|| PadInputRIGHT)
	{

		//------プレイヤーも同じ移動------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 1;

		if ((inputS)|| PadInputDOWN) {

			XMFLOAT3 PlayerPos = obj_player->GetPosition();
			PlayerPos.z = PlayerPos.z - moveSpeed;
			obj_player->SetPosition(PlayerPos);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x <= 10) {
				rotation.x += 1.f;
			}
			obj_player->SetRotation(rotation);

		}

		if ((inputW) || PadInputUP) {

			XMFLOAT3 PlayerPos = obj_player->GetPosition();
			PlayerPos.z = PlayerPos.z + moveSpeed;
			obj_player->SetPosition(PlayerPos);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x >= -10) {
				rotation.x -= 1.f;
			}
			obj_player->SetRotation(rotation);
		}

		if ((inputA) || PadInputLEFT) {

			XMFLOAT3 PlayerPos = obj_player->GetPosition();
			PlayerPos.x = PlayerPos.x - moveSpeed;
			obj_player->SetPosition(PlayerPos);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if ((inputD) || PadInputRIGHT) {

			XMFLOAT3 PlayerPos = obj_player->GetPosition();
			PlayerPos.x = PlayerPos.x + moveSpeed;
			obj_player->SetPosition(PlayerPos);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if (inputQ) {

			XMFLOAT3 PlayerPos = obj_player->GetPosition();
			PlayerPos.y = PlayerPos.y + moveSpeed;
			obj_player->SetPosition(PlayerPos);
		}

		if (inputZ) {

			XMFLOAT3 PlayerPos = obj_player->GetPosition();
			PlayerPos.y = PlayerPos.y - moveSpeed;
			obj_player->SetPosition(PlayerPos);
		}

	}
	if(TriggerR){//リセット
		obj_player->SetPosition({ 0,40,-170 });
		obj_player->SetRotation({ 0,0,0 });
	}

	//------------------↑プレイヤー移動＆姿勢

	XMFLOAT3 PlayerPosMemory = {};
	PlayerPosMemory = obj_player->GetPosition();//判定のためポジション入れる

	//自分回転
	if (inputE)
	{
		XMFLOAT3 rotation = obj_player->GetRotation();
		rotation.y++;
		obj_player->SetRotation(rotation);
	}

	//発射処理
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Update();
	}

	obj_player->Update();

}

void Player::Draw()
{
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Draw();
	}

	obj_player->Draw();
}
