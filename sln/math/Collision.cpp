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