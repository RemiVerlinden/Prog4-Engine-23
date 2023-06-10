#include "HealthComponent.h"
#include "GameObject.h"
#include "EventManager.h"
#include "TagComponent.h"

void engine::HealthComponent::SetMaxHealth(int maxHealth)
{
	m_Health.max = maxHealth;
}

void engine::HealthComponent::SetCurrentHealth(int health)
{
	m_Health.current = health;
}

void engine::HealthComponent::Damage(int damageAmount)
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

void engine::HealthComponent::Heal(int healAmount)
{
	m_Health.current += healAmount;
	if (m_Health.current > m_Health.max)
	{
		m_Health.current = m_Health.max;
	}
}

