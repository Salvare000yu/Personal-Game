#pragma once
#include "Primitive.h"
#include "Vector3.h"

//衝突判定
class Collision
{
public:

	//球と球
	static bool testSphereSphere(const Sphere& sphere1, const Sphere& sphere2);

	//球と直方体(AABB制限付き)
	static bool testSphereBox(const Sphere& sphere, const Box& box);

	//球とカプセル
	static bool testSphereCapsule(const Sphere& sphere, const Capsule& capsule);

	//カプセルとカプセル
	static bool testCapsuleCapsule(const Capsule& capsule1, const Capsule& capsule2);

	//線分と線分の距離の2乗
	static float sqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2);

	//値を範囲内に収める
	static float clamp(float x,float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? low : x;
		return x;
	}
};