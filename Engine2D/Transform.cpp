#include "Transform.h"
#include "GameObject.h"

using namespace engine;


const glm::vec3& engine::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		UpdateWorldPosition();
		m_IsDirty = false;
	}

	return m_WorldPosition;
}

void engine::TransformComponent::UpdateWorldPosition()
{
	GameObject* parent = m_GameObject->GetParent();

	if (!parent)
		m_WorldPosition = m_LocalPosition;
	else
		m_WorldPosition = m_LocalPosition + parent->m_Transform->GetWorldPosition();
}

TransformComponent& engine::TransformComponent::operator+=(const glm::vec3& offset)
{
	SetPositionDirty();

	TranslatePosition(offset);

	return *this;
}

void engine::TransformComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	auto componentClone = clone->AddComponent<TransformComponent>(GetComponentTag());
	componentClone->SetLocalPosition(m_LocalPosition);
}


void engine::TransformComponent::SetLocalPosition(const float x, const float y, const float z)
{
	SetPositionDirty();

	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
}

void engine::TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	SetPositionDirty();
	m_LocalPosition = pos;
}

void engine::TransformComponent::TranslatePosition(float x, float y, float z)
{
	SetPositionDirty();

	m_LocalPosition.x += x;
	m_LocalPosition.y += y;
	m_LocalPosition.z += z;
}

void engine::TransformComponent::TranslatePosition(const glm::vec3& pos)
{
	SetPositionDirty();

	m_LocalPosition += pos;
}

void engine::TransformComponent::SetPositionDirty()
{
	m_IsDirty = true;

	const auto& pChildVec = m_GameObject->GetChildren();
	for (GameObject* child : pChildVec)
	{
		child->m_Transform->SetPositionDirty();
	}
}
