#pragma once
#include "BaseComponent.h"
#include "IPhysicsSystem.hpp"

namespace engine
{
	class CirclePhysicsComponent final : public BaseComponent
	{
	public:
		virtual ~CirclePhysicsComponent() = default;

		virtual void Initialize();

		virtual void Update(const UpdateContext& pContext) override;
		virtual void Draw() override;
		Circle& GetCircleCollider() { return m_PhysicsCollider; }
		void CollisionOccured() { m_CollisionOccured = true; }
	protected:
		void Clone(GameObject*) override {};
	private:
		void UpdateColliderPosition();
		void UpdateObjectPosition();
		void ClampColliderPosToWindow();
		bool m_CollisionOccured = false;
		Circle m_PhysicsCollider;
		bool m_CanLeaveWindow = false;
	};
}