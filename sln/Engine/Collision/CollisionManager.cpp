#include "CollisionManager.h"
#include "BaseObject.h"
#include "Collision.h"
#include "CollisionPrimitive.h"

using namespace DirectX;

void CollisionManager::CheckHitFromColliderList(std::forward_list<Collider> list1,
	std::function<void(BaseObject*)> hitFunc1,
	std::forward_list<Collider> list2,
	std::function<void(BaseObject*)> hitFunc2)
{
	for (auto& i : list1)
	{
		if (!i.baseObject)continue;
		if (!i.baseObject->GetAlive())continue;

		const Sphere iSphere = Sphere{ .center = XMLoadFloat3(&i.baseObject->GetObj()->GetWorldPos()),.radius = i.radius };

		for (auto& j : list2)
		{
			if (!j.baseObject)continue;
			if (!j.baseObject->GetAlive())continue;

			const Sphere jSphere = Sphere{ .center = XMLoadFloat3(&j.baseObject->GetObj()->GetWorldPos()),.radius = j.radius };

			if (Collision::CheckSphere2Sphere(iSphere, jSphere)) {
				hitFunc1(i.baseObject);
				hitFunc2(j.baseObject);
			}
		}
	}
}