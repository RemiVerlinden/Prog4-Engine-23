#pragma once
#include "IPhysicsSystem.hpp"

namespace dae
{
	class PhysicsSystem final : public IPhysicsSystem
	{
	public:
		PhysicsSystem() = default;
		virtual ~PhysicsSystem() = default;

		virtual bool CheckLevelCollision(GameObject* pObject) const;
		virtual bool CheckLevelCollision(ColliderComponent* pCollider) const;

		virtual void AddCollider(ColliderComponent* pCollider);
		virtual void AddCollider(GameObject* pObject);

		virtual void RemoveCollider(ColliderComponent* pCollider);
		virtual void RemoveCollider(GameObject* pObject);
	private:
		bool CheckCollisionAABB(const Rect& a, const Rect& b) const;

	};
}
