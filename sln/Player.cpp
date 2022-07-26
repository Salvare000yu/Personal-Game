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
	//camera.reset(new Camera(WinApp::window_width, WinApp::window_height));
	// カメラセット
	//Object3d::SetCamera(camera.get());

	mod_classplayer.reset(Model::LoadFromOBJ("bullet2"));

	obj_classplayer.reset(Object3d::Create());

	obj_classplayer->SetModel(mod_classplayer.get());

	obj_classplayer->SetScale({ 20.0f, 20.0f, 20.0f });

	obj_classplayer->SetPosition({ 0,100,-0 });
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	const bool input3 = input->PushKey(DIK_E);

	if (input3) {
		XMFLOAT3 position = obj_classplayer->GetPosition();
		position.z += 5;
		obj_classplayer->SetPosition(position);
	}

	//黄金の回転
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 rotation = obj_classplayer->GetRotation();
		rotation.y += 0.7f;
		rotation.x += 0.4f;
		obj_classplayer->SetRotation({ rotation });
	}

	obj_classplayer->Update();
}

void Player::Draw()
{
	obj_classplayer->Draw();
}
