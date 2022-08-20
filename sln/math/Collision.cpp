#include "Primitive.h"
#include "Vector3.h"
#include "Collision.h"
#include "Quaternion.h"

//���Ƌ��̏Փ˔���
bool Collision::testSphereSphere(const Sphere& sphere1, const Sphere& sphere2)
{
	//2�̋��̒��S���W�����ԃx�N�g�������߂� pb��-pa
	Vector3 vec = sphere2.position - sphere1.position;

	//�x�N�g���̒����̓��i2�_�Ԃ̋����̓��j�����߂�
	float sqLength = vec.x*vec.x+vec.y*vec.y+vec.z*vec.z;

	//2�̋��̔��a�̍��v
	float r = sphere1.radius+sphere2.radius;
	//radius+radius

	//2�_�Ԃ̋�����2�恃���a�̍��v��2��Ȃ�Q�̋��͌������Ă���
	return sqLength<r*r;
}

//���ƒ����́iAABB�j�̏Փ˔���
bool Collision::testSphereBox(const Sphere& sphere, const Box& box)
{
	//�܂��A���̒��S���W�̂�AABB�̋�����2������߂�-------
	float sqDistance = 0.0f;
	float pos;

	//x�������̋�����2������Z
	pos = sphere.position.x;
	if (pos < box.minPosition.x)
	{
		sqDistance += (box.minPosition.x - pos) * (box.minPosition.x - pos);
	}
	else if (pos > box.maxPosition.x)
	{
		sqDistance += (box.maxPosition.x-pos)* (box.maxPosition.x - pos);
	}

	//y�������̋�����2������Z
	pos = sphere.position.y;
	if (pos < box.minPosition.y)
	{
		sqDistance += (box.minPosition.y-pos)* (box.minPosition.y - pos);
	} else if (pos > box.maxPosition.y)
	{
		sqDistance += (box.maxPosition.y-pos)* (box.maxPosition.y - pos);
	}
	//z�������̋�����2������Z
	pos = sphere.position.z;
	if (pos < box.minPosition.z)
	{
		sqDistance += (box.minPosition.z-pos)* (box.minPosition.z - pos);
	} else if (pos > box.maxPosition.z)
	{
		sqDistance += (box.maxPosition.z-pos)* (box.maxPosition.z - pos);
	}
	//��L�ŋ��߂��l�����a��2��Ȃ�Փ˂��Ă���---------------
	return sqDistance<sphere.radius*sphere.radius;
}

bool Collision::testSphereCapsule(const Sphere& sphere, const Capsule& capsule)
{
	//1�@�J�v�Z�����̃X�^�[�g�ʒu����G���h�ʒu�ւ̃x�N�g���쐬
	//�l����
	Vector3 vStartToEnd = capsule.endPosition-capsule.startPosition;

	//2�@1�̃x�N�g����P�ʃx�N�g�����inormalize�j���An�ɗp�ӂ���
	Vector3 n = vStartToEnd;
	n.normalize();

	//3 Ps->Pc�ւ̃x�N�g����2�ŋ��߂�n�Ƃ̓��όv�Z
	//n�����{����΃x�N�g��Ps�[��Pn�ɂȂ邩�{���������܂�
	float t = (sphere.position-capsule.startPosition).dot(n);

	//4 Ps->pn�x�N�g�������߂Ă����APn�̍��W�����߂Ă���
	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition+vPsPn;

	// 5�䗦t/(Ps->Pe�̒���)�����߂�
	float lengthRate = t / vStartToEnd.length();

	//6lengthRate<0,0<=lengthRate<=1,1<lengthRate�ŏꍇ����
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

	//�����Ɛ����̋����𒲂ׂ�@�����ʂ͋�����2��
	float sqDistance =
		sqDistanceSegmentSegment(capsule1.startPosition,capsule1.endPosition,capsule2.startPosition,capsule2.endPosition);

	//2�̃J�v�Z���̔��a�̘a
	float r = capsule1.radius+capsule2.radius;

	//�Փ˔���̌��ʂ�Ԃ�
	return sqDistance<r * r;
}

float Collision::sqDistanceSegmentSegment(const Vector3& p1, const Vector3& q1,
	const Vector3& p2, const Vector3& q2)
{
	Vector3 d1 = q1 - p1;//p1->q1�x�N�g��
	Vector3 d2 = q2 - p2;//p2->q2�x�N�g��

	Vector3 r = p1 - p2;

	float a = d1.dot(d1);//a=d1*d1
	float b = d1.dot(d2);//b=d1*d2
	float e = d2.dot(d2);//e=d2*d2

	float c = d1.dot(r);//c=d1*r
	float f = d2.dot(r);//f=d2*r

	float s = 0.0f;
	float t = 0.0f;

	float denominator = (a * e) - (b * b);//����

	//s�̒l
	if (denominator != 0.0f)//2�̐��������s�łȂ��Ȃ�
	{
		s = ((b * f) - (c * e)) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	} else
	{
		s = 0.0f;
	}

	//t�̒l
	//t=((p1+d1*s)-p2)*d2/(d2*d2)
	t = (f + b * s) / e;

	//t��[0.0,1.0]�͈͓̔��ł����s�Čv�Z����
	//s��((p2+d2*t)-p1)*d1/(d1*d1)=(t*b-c)/a
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
	//s��t�̒l�����肵���̂Ŋe�������̍��Wc1��c2�����߂�
	Vector3 c1 = p1+s*d1;
	Vector3 c2 = p2+t*d2;

	//2�_�ԁic1��c2�j�̋�����2������߂Č��ʂ�Ԃ�
	return (c1-c2).dot(c1-c2);
}