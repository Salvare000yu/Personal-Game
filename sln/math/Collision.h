#pragma once
#include "Primitive.h"
#include "Vector3.h"

//�Փ˔���
class Collision
{
public:

	//���Ƌ�
	static bool testSphereSphere(const Sphere& sphere1, const Sphere& sphere2);

	//���ƒ�����(AABB�����t��)
	static bool testSphereBox(const Sphere& sphere, const Box& box);

	//���ƃJ�v�Z��
	static bool testSphereCapsule(const Sphere& sphere, const Capsule& capsule);
};