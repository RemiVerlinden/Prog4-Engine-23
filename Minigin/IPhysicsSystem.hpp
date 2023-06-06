#pragma once
#include <vector>

namespace dae
{
	class GameObject;
	class ColliderComponent;

	struct Rect
	{
		int x, y;
		int w, h;
	};

	class IPhysicsSystem	
	{
	public:
		IPhysicsSystem() = default;
		virtual ~IPhysicsSystem() = default;

		virtual bool CheckLevelCollision(GameObject* pObject) const = 0;
		virtual bool CheckLevelCollision(ColliderComponent* pCollider) const = 0;

		virtual void AddCollider(ColliderComponent* pCollider) = 0;
		virtual void AddCollider(GameObject* pObject) = 0;

		virtual void RemoveCollider(ColliderComponent* pCollider) = 0;
		virtual void RemoveCollider(GameObject* pObject) = 0;

	protected:
		std::vector<ColliderComponent*> m_pColliders;
	};
}