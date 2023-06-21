//当たり判定を行うクラス

#pragma once
#include <functional>
#include <forward_list>

class BaseObject;

class CollisionManager
{
public:
	struct Collider {
		BaseObject* baseObject = nullptr;
		float radius = 1.f;
	};

	static void CheckHitFromColliderList(
		std::forward_list<Collider> list1,
		std::function<void(BaseObject*)> hitFunc1,
		std::forward_list<Collider> list2,
		std::function<void(BaseObject*)> hitFunc2);
};
