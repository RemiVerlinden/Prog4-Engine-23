#include "MoveComponent.h"
#include "GameObject.h"
#include "Transform.h"

void engine::MoveComponent::Initialize()
{
	m_Transform = m_GameObject->m_Transform;
}

void engine::MoveComponent::Update(const UpdateContext& context)
{
	
	glm::vec2 moveVec = m_MoveDirection * m_Speed * context.GetDeltaTime().ToFloat();
	*m_Transform += glm::vec3(moveVec.x, moveVec.y, 0);

	m_MoveDirection = glm::vec2{0,0};
}

void engine::MoveComponent::SetMoveDirection(glm::vec2 direction)
{
	m_MoveDirection += direction;

	if (glm::length(m_MoveDirection) > 1.0f)
	{
		m_MoveDirection = glm::normalize(m_MoveDirection);
	}
}

