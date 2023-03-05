#include "OrbitComponent.h"
#include "GameObject.h"

using namespace dae;

void dae::OrbitComponent::Initialize(GameTime* time)
{
	m_Time = time;
	m_Transform = m_GameObject->GetComponent<TransformComponent>().lock().get();
	m_CenterPos = m_Transform->GetWorldPosition();
}

void dae::OrbitComponent::Update()
{
	float time = m_Time->GetTotal();

	glm::vec3 orbitPos{ m_CenterPos };
	orbitPos.x += glm::cos(m_Speed * time) * m_Radius;
	orbitPos.y += glm::sin(m_Speed * time) * m_Radius;

	m_Transform->SetLocalPosition(orbitPos);
}

void dae::OrbitComponent::Draw()
{
}

void dae::OrbitComponent::LateUpdate()
{
}

void dae::OrbitComponent::FixedUpdate()
{
}
