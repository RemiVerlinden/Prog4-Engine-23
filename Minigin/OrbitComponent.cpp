#include "OrbitComponent.h"
#include "GameObject.h"
#include "UpdateContext.h"

using namespace dae;

void dae::OrbitComponent::Initialize()
{
	m_Transform = m_GameObject->GetComponent<TransformComponent>().lock().get();
	m_CenterPos = m_Transform->GetWorldPosition();
}

void dae::OrbitComponent::Update([[maybe_unused]] const UpdateContext& context)
{
	float time = EngineClock::GetTimeInSeconds();

	glm::vec3 orbitPos{ m_CenterPos };
	orbitPos.x += glm::cos(m_Speed * time) * m_Radius;
	orbitPos.y += glm::sin(m_Speed * time) * m_Radius;

	m_Transform->SetLocalPosition(orbitPos);
}

void dae::OrbitComponent::Draw()
{
}

void dae::OrbitComponent::LateUpdate([[maybe_unused]] const UpdateContext& context)
{
}

void dae::OrbitComponent::FixedUpdate([[maybe_unused]] const UpdateContext& context)
{
}
