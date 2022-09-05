#include "BaseObject.h"

void BaseObject::Initialize()
{
	obj->Initialize();
}

void BaseObject::Update()
{
	obj->Update();
}

void BaseObject::Draw()
{
	if (alive) {
		obj->Draw();
	}
}