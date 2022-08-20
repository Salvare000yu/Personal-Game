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

bool Collision::testCapsuleCapsule(const Capsule& capsule1, const Capsule& capsule2)
{

	//線分と線分の距離を調べる　＊結果は距離の2乗
	float sqDistance =
		sqDistanceSegmentSegment(capsule1.startPosition,capsule1.endPosition,capsule2.startPosition,capsule2.endPosition);

	//2つのカプセルの半径の和
	float r = capsule1.radius+capsule2.radius;

	//衝突判定の結果を返す
	return sqDistance<r * r;
}

float Collision::sqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1,
	const Vector3& p2, const Vector3& q2)
{
	Vector3 d1 = q1 - p1;//p1->q1ベクトル
	Vector3 d2 = q2 - p2;//p2->q2ベクトル

	Vector3 r = p1 - p2;

	float a = d1.dot(d1);//a=d1*d1
	float b = d1.dot(d2);//b=d1*d2
	float e = d2.dot(d2);//e=d2*d2

	float c = d1.dot(r);//c=d1*r
	float f = d2.dot(r);//f=d2*r

	float s = 0.0f;
	float t = 0.0f;

	float denominator = (a * e) - (b * b);//分母

	//sの値
	if (denominator != 0.0f)//2つの線分が平行でないなら
	{
		s = ((b * f) - (c * e)) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	} else
	{
		s = 0.0f;
	}

	//tの値
	//t=((p1+d1*s)-p2)*d2/(d2*d2)
	t = (f + b * s) / e;

	//tが[0.0,1.0]の範囲内であればs再計算する
	//s＝((p2+d2*t)-p1)*d1/(d1*d1)=(t*b-c)/a
	if (t < 0.0f)
	{
		t = 0.0f;
		s = clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		s = clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	//sとtの値が決定したので各線分内の座標c1とc2を求める
	Vector3 c1 = p1+s*d1;
	Vector3 c2 = p2+t*d2;

	//2点間（c1とc2）の距離の2乗を求めて結果を返す
	return (c1-c2).dot(c1-c2);
}