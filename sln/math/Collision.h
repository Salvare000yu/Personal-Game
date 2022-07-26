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
};