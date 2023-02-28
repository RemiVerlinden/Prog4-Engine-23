#include "Transform.h"

void dae::TransformComponent::Initialize([[maybe_unused]]GameTime* time)
{
}

void dae::TransformComponent::Update()
{
}

void dae::TransformComponent::Draw()
{
}

void dae::TransformComponent::LateUpdate()
{
}

void dae::TransformComponent::FixedUpdate()
{
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
