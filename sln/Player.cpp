#include "Player.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
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

	const bool TriggerR = input->TriggerKey(DIK_R);

	time = frame / 60.f;

	//----------↓移動制限 カメラと同じにする
	const float PlayerMoveLimX = 190;

	const float PlayerMaxMoveLimY = 100;//下に行ける範囲
	const float PlayerMinMoveLimY = 200;//上に行ける範囲

	const float PlayerMaxMoveLimZ = 290;//後ろ
	const float PlayerMinMoveLimZ = 200;

	XMFLOAT3 position = obj_player->GetPosition();
	position.x = max(position.x, -PlayerMoveLimX);
	position.x = min(position.x, +PlayerMoveLimX);
	position.y = max(position.y, -PlayerMaxMoveLimY);//下に行ける範囲
	position.y = min(position.y, +PlayerMinMoveLimY);//上に行ける範囲
	position.z = max(position.z, -PlayerMaxMoveLimZ);
	position.z = min(position.z, +PlayerMinMoveLimZ);
	obj_player->SetPosition(position);
	//----------↑移動制限

	//------------------↓プレイヤー移動＆姿勢
	if (inputW || inputS || inputA || inputD || inputQ || inputZ)
	{

		//------プレイヤーも同じ移動------//
		bool OldInputFlag = FALSE;
		constexpr float moveSpeed = 1;

		if (inputS) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x <= 10) {
				rotation.x += 1.f;
			}
			obj_player->SetRotation(rotation);

		}

		if (inputW) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.z = position.z + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.x >= -10) {
				rotation.x -= 1.f;
			}
			obj_player->SetRotation(rotation);
		}

		if (inputA) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.x = position.x - moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z <= 10) {
				rotation.z += 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if (inputD) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.x = position.x + moveSpeed;
			obj_player->SetPosition(position);

			XMFLOAT3 rotation = obj_player->GetRotation();
			if (rotation.z >= -10) {
				rotation.z -= 1.f;
			}
			obj_player->SetRotation(rotation);

			OldInputFlag = TRUE;
		}

		if (inputQ) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.y = position.y + moveSpeed;
			obj_player->SetPosition(position);
		}

		if (inputZ) {

			XMFLOAT3 position = obj_player->GetPosition();
			position.y = position.y - moveSpeed;
			obj_player->SetPosition(position);
		}

	}
	if(TriggerR){//リセット
		obj_player->SetPosition({ 0,40,-170 });
		obj_player->SetRotation({ 0,0,0 });
	}

	//------------------↑プレイヤー移動＆姿勢

	//自分回転
	if (inputE)
	{
		XMFLOAT3 rotation = obj_player->GetRotation();
		rotation.y++;
		obj_player->SetRotation(rotation);
	}

	obj_player->Update();

}

void Player::Draw()
{
	obj_player->Draw();
}
