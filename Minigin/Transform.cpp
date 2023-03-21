#include "Transform.h"
#include "GameObject.h"

using namespace dae;


const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		UpdateWorldPosition();
		m_IsDirty = false;
	}

	return m_WorldPosition;
}

void dae::TransformComponent::UpdateWorldPosition()
{
	GameObject* parent = m_GameObject->GetParent();

	if (!parent)
		m_WorldPosition = m_LocalPosition;
	else
		m_WorldPosition = m_LocalPosition + parent->m_Transform->GetWorldPosition();
}

TransformComponent& dae::TransformComponent::operator+=(const glm::vec3& offset)
{
	SetPositionDirty();

	TranslatePosition(offset);

	return *this;
}


void dae::TransformComponent::SetLocalPosition(const float x, const float y, const float z)
{
	SetPositionDirty();

	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	SetPositionDirty();
	m_LocalPosition = pos;
}

void dae::TransformComponent::TranslatePosition(float x, float y, float z)
{
	SetPositionDirty();

	m_LocalPosition.x += x;
	m_LocalPosition.y += y;
	m_LocalPosition.z += z;
}

void dae::TransformComponent::TranslatePosition(const glm::vec3& pos)
{
	SetPositionDirty();

	m_LocalPosition += pos;
}

void dae::TransformComponent::SetPositionDirty()
{
	m_IsDirty = true;

	const auto& pChildVec = m_GameObject->GetChildren();
	for (GameObject* child : pChildVec)
	{
		child->m_Transform->SetPositionDirty();
	}
}
