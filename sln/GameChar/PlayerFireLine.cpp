#include "PlayerFireLine.h"

using namespace DirectX;

void PlayerFireLine::Initialize()
{
	//’è‹`‚Æ‚©‰¼‚¨‚¢‚Ä‚¨‚±‚¤
	//ì‚é
	obj.reset(Object3d::Create());
	//-----«”CˆÓ«-----//
	//‘å‚«‚³
	obj->SetScale({ 3.f, 3.f, 3.f });
	//êŠ

	//obj->SetPosition({ BulletPos });

}

void PlayerFireLine::Update()
{
	XMFLOAT3 FireLinePos = obj->GetPosition();

	obj->SetPosition(FireLinePos);

	obj->Update();
}

void PlayerFireLine::Draw()
{
	obj->Draw();
}
