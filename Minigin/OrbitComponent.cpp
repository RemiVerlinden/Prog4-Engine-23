#include "OrbitComponent.h"
#include "GameObject.h"
#include "UpdateContext.h"

using namespace dae;

void dae::OrbitComponent::Initialize()
{
	m_CenterPos = m_GameObject->m_Transform->GetWorldPosition();
}

void dae::OrbitComponent::Update([[maybe_unused]] const UpdateContext& context)
{
	float time = EngineClock::GetTimeInSeconds();

	glm::vec3 orbitPos{ m_CenterPos };
	orbitPos.x += glm::cos(m_Speed * time) * m_Radius;
	orbitPos.y += glm::sin(m_Speed * time) * m_Radius;

	m_GameObject->m_Transform->SetLocalPosition(orbitPos);
}

void dae::OrbitComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	auto componentClone = clone->AddComponent<OrbitComponent>(GetComponentTag());
	componentClone->SetSpeed(m_Speed);
	componentClone->SetRadius(m_Radius);
}
