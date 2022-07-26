#include "Primitive.h"
#include "Vector3.h"
#include "Collision.h"
#include "Quaternion.h"

//球と球の衝突判定
bool Collision::testSphereSphere(const Sphere& sphere1, const Sphere& sphere2)
{
	//2つの球の中心座標を結ぶベクトルを求める pb→-pa
	Vector3 vec = sphere2.position - sphere1.position;

	//ベクトルの長さの二乗（2点間の距離の二乗）を求める
	float sqLength = vec.x*vec.x+vec.y*vec.y+vec.z*vec.z;

	//2つの球の半径の合計
	float r = sphere1.radius+sphere2.radius;
	//radius+radius

	//2点間の距離の2乗＜半径の合計の2乗なら２つの球は交差している
	return sqLength<r*r;
}

//球と直方体（AABB）の衝突判定
bool Collision::testSphereBox(const Sphere& sphere, const Box& box)
{
	//まず、球の中心座標のとAABBの距離の2乗を求める-------
	float sqDistance = 0.0f;
	float pos;

	//x軸方向の距離の2乗を加算
	pos = sphere.position.x;
	if (pos < box.minPosition.x)
	{
		sqDistance += (box.minPosition.x - pos) * (box.minPosition.x - pos);
	}
	else if (pos > box.maxPosition.x)
	{
		sqDistance += (box.maxPosition.x-pos)* (box.maxPosition.x - pos);
	}

	//y軸方向の距離の2乗を加算
	pos = sphere.position.y;
	if (pos < box.minPosition.y)
	{
		sqDistance += (box.minPosition.y-pos)* (box.minPosition.y - pos);
	} else if (pos > box.maxPosition.y)
	{
		sqDistance += (box.maxPosition.y-pos)* (box.maxPosition.y - pos);
	}
	//z軸方向の距離の2乗を加算
	pos = sphere.position.z;
	if (pos < box.minPosition.z)
	{
		sqDistance += (box.minPosition.z-pos)* (box.minPosition.z - pos);
	} else if (pos > box.maxPosition.z)
	{
		sqDistance += (box.maxPosition.z-pos)* (box.maxPosition.z - pos);
	}
	//上記で求めた値＜半径の2乗なら衝突している---------------
	return sqDistance<sphere.radius*sphere.radius;
}

bool Collision::testSphereCapsule(const Sphere& sphere, const Capsule& capsule)
{
	//1　カプセル内のスタート位置からエンド位置へのベクトル作成
	//考えて
	Vector3 vStartToEnd = capsule.endPosition-capsule.startPosition;

	//2　1のベクトルを単位ベクトル化（normalize）し、nに用意する
	Vector3 n = vStartToEnd;
	n.normalize();

	//3 Ps->Pcへのベクトルと2で求めたnとの内積計算
	//nを何倍すればベクトルPsー＞Pnになるか倍率ｔが求まる
	float t = (sphere.position-capsule.startPosition).dot(n);

	//4 Ps->pnベクトルを求めておく、Pnの座標も求めておく
	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition+vPsPn;

	// 5比率t/(Ps->Peの長さ)を求める
	float lengthRate = t / vStartToEnd.length();

	//6lengthRate<0,0<=lengthRate<=1,1<lengthRateで場合分け
	float distance;
	if (lengthRate < 0.0f)
	{
		distance = (sphere.position - capsule.startPosition).length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = (sphere.position - posPn).length() -capsule.radius;
	}
	else
	{
		distance = (sphere.position - capsule.endPosition).length() -capsule.radius;
	}
	
	return distance<sphere.radius;
}