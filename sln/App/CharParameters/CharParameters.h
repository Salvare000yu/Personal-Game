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
	//自機喰らった
	void SetispDam(bool ispDam) { this->ispDam = ispDam; }
	const bool& GetispDam() { return ispDam; }
	//-----------------------↑げったーせったー↑-----------------------//

	const int stopPos = 1800;//自機次座標への移動の停止場所
	//次の場所へ向かう　true：向かっている false:完了
	bool pNextPlaceGoFlag = true;

private:

	//自機がダメージ喰らった false:喰らってない
	bool ispDam = false;

};
