#include "TemplateComponent.h"
#include "ColliderComponent.h"
#include "Locator.h"
#include "GameObject.h"

using namespace engine::physics;

void engine::ColliderComponent::Initialize()
{
	Scene* pScene = GetGameObject()->GetScene();
	pScene->GetPhysicsWorld().AddCollider(this);
}

engine::ColliderComponent::~ColliderComponent()
{
	Scene* pScene = GetGameObject()->GetScene();
	pScene->GetPhysicsWorld().RemoveCollider(this);
}


void engine::ColliderComponent::SetCollider(const Shape& collider)
{
	m_IsInitialized = true;
	m_Collider = collider;
}

const Shape& engine::ColliderComponent::GetCollider() const
{
	if(!m_IsInitialized) ENGINE_WARN("ColliderComponent::GetCollider() > ColliderComponent not initialized");
	return m_Collider;
}

void engine::ColliderComponent::SetPosition(glm::vec2 position)
{
	if (!m_IsInitialized) ENGINE_WARN("ColliderComponent::SetPosition() > ColliderComponent not initialized");

	std::visit([this, position](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;
		T new_arg = arg;
		new_arg.pos = position;
		m_Collider = new_arg;
		}, m_Collider);
}
