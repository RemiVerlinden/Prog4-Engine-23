#include "HealthComponent.h"
#include "GameObject.h"
#include "EventManager.h"
#include "TagComponent.h"

void dae::HealthComponent::SetMaxHealth(int maxHealth)
{
	m_Health.max = maxHealth;
}

void dae::HealthComponent::SetCurrentHealth(int health)
{
	m_Health.current = health;
}

void dae::HealthComponent::Damage(int damageAmount)
{
	auto& eventManager = EventManager::GetInstance();
	TagComponent* tagComp = m_GameObject->GetComponent<TagComponent>();


	m_Health.current -= damageAmount;
	
	
	if (m_Health.current <= 0)
	{
		eventManager.SendEvent(OnDeathEvent{ tagComp->m_Tag });
		m_Health.current = 0;
		m_GameObject->Destroy();
	}
	eventManager.SendEvent(OnDamageEvent{tagComp->m_Tag, m_Health.current, damageAmount});
}

void dae::HealthComponent::Heal(int healAmount)
{
	m_Health.current += healAmount;
	if (m_Health.current > m_Health.max)
	{
		m_Health.current = m_Health.max;
	}
}

void dae::HealthComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;

	auto componentClone = clone->AddComponent<HealthComponent>(GetComponentTag());
	componentClone->SetMaxHealth(m_Health.max);
	componentClone->SetCurrentHealth(m_Health.current); 
}
