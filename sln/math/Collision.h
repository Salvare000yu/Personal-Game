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

	//�J�v�Z���ƃJ�v�Z��
	static bool testCapsuleCapsule(const Capsule& capsule1, const Capsule& capsule2);

	//�����Ɛ����̋�����2��
	static float sqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1, const Vector3& p2, const Vector3& q2);

	//�l��͈͓��Ɏ��߂�
	static float clamp(float x,float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? low : x;
		return x;
	}
};