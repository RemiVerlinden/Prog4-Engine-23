#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"

namespace engine
{
	class TransformComponent;
	class MoveComponent final : public BaseComponent
	{
	public:
		virtual void Initialize() override;

		virtual void Update(const UpdateContext& context);

		void SetMoveDirection(glm::vec2 direction);
		void SetMoveSpeed(float speed) { m_Speed = speed; };
	private:
		TransformComponent* m_Transform;
		glm::vec2 m_MoveDirection;
		float m_Speed = 100.f;


	private:
		void Clone(GameObject* clone) override;
	};
}

