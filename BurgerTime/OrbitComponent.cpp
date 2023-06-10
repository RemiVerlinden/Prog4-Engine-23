#include "OrbitComponent.h"
#include "GameObject.h"
#include "UpdateContext.h"

using namespace engine;

void engine::OrbitComponent::Initialize()
{
	m_CenterPos = m_GameObject->m_Transform->GetWorldPosition();
}

void engine::OrbitComponent::Update(const UpdateContext& /*context*/)
{
	float time = EngineClock::GetTimeInSeconds();

	glm::vec3 orbitPos{ m_CenterPos };
	orbitPos.x += glm::cos(m_Speed * time) * m_Radius;
	orbitPos.y += glm::sin(m_Speed * time) * m_Radius;

	m_GameObject->m_Transform->SetLocalPosition(orbitPos);
}

