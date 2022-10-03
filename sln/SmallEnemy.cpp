#include "SmallEnemy.h"
#include "Object3d.h"
#include "Input.h"

#include "GameSound.h"

#include <DirectXMath.h>
#include <random>

void SmallEnemy::Initialize()
{
	std::srand((unsigned)std::time(nullptr));

	//çÏÇÈ
	obj.reset(Object3d::Create());
	//-----Å´îCà”Å´-----//
	//ëÂÇ´Ç≥
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	obj->SetRotation({ 1.0f, 270.0f, 1.0f });
	//èÍèä

	SEneRandX = float(rand() % 100 - 50);
	obj->SetPosition({ SEneRandX,40,400 });

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
	for (int i = 0; i < 1; i++)
	{
		XMFLOAT3 smEnemPos = obj->GetPosition();
		smEnemPos.z -= 4;
		obj->SetPosition(smEnemPos);

	}

	//éûä‘åoâﬂè¡ñ≈
	if (--vanishTimer_ <= 0) { alive = false; }

	obj->Update();
}

void SmallEnemy::Draw()
{
	if (alive) {
		obj->Draw();
	}
}
