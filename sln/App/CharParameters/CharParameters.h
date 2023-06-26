#pragma once
#include "BaseObject.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Player.h"

class CharParameters :public BaseObject
{
public:
	static CharParameters* GetInstance();
	void Initialize() override;
	void Update()override;

	void Draw()override;
	void DrawUI();

	//-----------------------↓げったーせったー↓-----------------------//

	//-----------------------↑げったーせったー↑-----------------------//



private:



};
