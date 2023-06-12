#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"
#include "IPhysicsSystem.hpp" // for Shape

namespace engine
{
	class ColliderComponent final : public BaseComponent
	{
	public:
		void Initialize();
		virtual ~ColliderComponent();
		void SetCollider(const physics::Shape& collider);
		const physics::Shape& GetCollider() const;
		void SetPosition(glm::vec2 position);

		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;
	private:
		physics::Shape m_Collider;
		
		bool m_IsInitialized = false;
	};
}

