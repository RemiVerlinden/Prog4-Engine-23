#pragma once
#include <vector>
#include "ColliderComponent.h"
#include "CirclePhysicsComponent.h"

namespace dae
{
	class ColliderComponent;
	class IWorldPhysics
	{
	public:
		virtual ~IWorldPhysics() = default;

		virtual void SolveWorldCollision(CirclePhysicsComponent*) = 0;
		void AddCollider(ColliderComponent* pCollider) { m_WorldColliders.emplace_back(pCollider); }
		void RemoveCollider(ColliderComponent* pCollider) {	std::erase(m_WorldColliders, pCollider); }
	private:
		std::vector<ColliderComponent*> m_WorldColliders;
	};
}
