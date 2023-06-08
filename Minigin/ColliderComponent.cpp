#include "TemplateComponent.h"
#include "ColliderComponent.h"
#include "Locator.h"
#include "GameObject.h"
void dae::ColliderComponent::Initialize()
{
	Scene* pScene = GetGameObject()->GetScene();
	pScene->GetPhysicsWorld().AddCollider(this);
}

dae::ColliderComponent::~ColliderComponent()
{
	Scene* pScene = GetGameObject()->GetScene();
	pScene->GetPhysicsWorld().RemoveCollider(this);
}


void dae::ColliderComponent::SetCollider(const Shape& collider)
{
	m_IsInitialized = true;
	m_Collider = collider;
}

const dae::Shape& dae::ColliderComponent::GetCollider() const
{
	if(!m_IsInitialized) ENGINE_WARN("ColliderComponent::GetCollider() > ColliderComponent not initialized");
	return m_Collider;
}

void dae::ColliderComponent::SetPosition(glm::vec2 position)
{
	if (!m_IsInitialized) ENGINE_WARN("ColliderComponent::SetPosition() > ColliderComponent not initialized");

	std::visit([this, position](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;
		T new_arg = arg;
		new_arg.pos = position;
		m_Collider = new_arg;
		}, m_Collider);
}
