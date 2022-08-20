#pragma once
#include "Primitive.h"
#include "Vector3.h"

struct Sphere
{
	Vector3 position;//中心座標
	float radius;//半径

	unsigned color;//表示色

	//コンストラクタ
	Sphere(const Vector3& position, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);//1球の情報表示
};

struct Box
{
	//xyzのそれぞれの最小値と最大値のデータとして持たせる方法
	Vector3 minPosition;//最小値xyz
	Vector3 maxPosition;//最大値xyz

	//中心座標とサイズとして持たせる方法もある
	//Vector3 position;//中心座標
	//float half[3];//x方向のサイズ/2、y方向のサイズ/2、z方向のサイズ/2

	unsigned color;//表示色

	//コンストラクタ
	Box(const Vector3& minPosition, const Vector3& maxPosition, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);//直方体の情報表示
};
//カプセル
struct Capsule
{
	Vector3 startPosition;
	Vector3 endPosition;
	float radius;

	unsigned color;//表示色

	//コンストラクタ
	Capsule(const Vector3& startPosition, const Vector3& endPosition, float radius, unsigned color);

	void draw();//描画
	void drawInfo(int x, int y, unsigned color);//カプセルの情報表示
};
