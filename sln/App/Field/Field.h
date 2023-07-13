#pragma once
/// <summary>
/// フィールド(戦う場所地面とトンネル)クラス
/// </summary>
#include "Object3d.h"

#include <unordered_map>

class Field
{
public:
	static Field* GetInstance();
	void Initialize();
	void Update();
	void Draw();

	void GroundUpDown();//地面上下
	void GroundAdvance();//地面前進
	void TunnelMove();//トンネルの動き

	void Departure();//発進
private:
	std::unique_ptr < Model> mod_groundBottom = nullptr;//した地面
	std::unique_ptr < Object3d> obj_groundBottom = nullptr;
	std::unordered_map <std::string, std::unique_ptr < Model>> mod_ground;//地面
	std::unordered_map <std::string, std::unique_ptr < Object3d>> obj_ground;

	std::unique_ptr < Model> mod_tunnel = nullptr;//トンネル
	std::unique_ptr < Object3d> obj_tunnel = nullptr;

	DirectX::XMFLOAT3 fieldScale;//トンネルと地面の大きさ

	//<<<<<地面
	float groundPosDef;//地面を置く座標　ここ中心に上下
	float swingSp;//揺らす速度
	float swingDist;//揺らす距離
	uint32_t groundMoveFrame = 0;
	float groundMoveTime = 0;

	float shiftSpeed;//UVシフトの速度

};

