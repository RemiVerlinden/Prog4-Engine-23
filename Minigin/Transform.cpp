#include "Transform.h"
#include "GameObject.h"

using namespace dae;

void dae::TransformComponent::Initialize()
{
}

void dae::TransformComponent::Update([[maybe_unused]] const UpdateContext& context)
{
	GetWorldPosition();
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		m_IsDirty = false;
		//return local position if the gameobject we are attached to has no parent
		GameObject* parent = m_GameObject->GetParent();
		if (parent == nullptr) 
		{
			m_WorldPosition = m_LocalPosition;
			return m_LocalPosition;
		}

		//ask parent for its' world position, and add it to ours
		//this way, if our parent has another parent, it will calculate its own world position
		const glm::vec3 parentWorldPosition = parent->m_Transform->GetWorldPosition();
		m_WorldPosition = m_LocalPosition + parentWorldPosition;
	}

	return m_WorldPosition;
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

void dae::TransformComponent::SetPositionDirty()
{
	m_IsDirty = true;

	const auto& pChildVec = m_GameObject->GetChildren();
	for (GameObject* child : pChildVec)
	{
		child->m_Transform->SetPositionDirty();
	}
}
