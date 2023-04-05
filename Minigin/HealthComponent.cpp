#include "HealthComponent.h"
#include "GameObject.h"
#include "EventManager.h"
#include "TagComponent.h"

void dae::HealthComponent::SetHealth(int maxHealth)
{
	m_Health.max = maxHealth;
}

void dae::HealthComponent::Damage(int damageAmount)
{
	auto& eventManager = EventManager::GetInstance();
	TagComponent* tagComp = m_GameObject->GetComponent<TagComponent>();


	m_Health.current -= damageAmount;
	if (m_Health.current <= 0)
	{
		eventManager.SendEvent(OnDeathEvent{ tagComp->m_Tag });
		m_GameObject->Destroy();
	}
	else
	{
		eventManager.SendEvent(OnDamageEvent{tagComp->m_Tag, m_Health.current, damageAmount});
	}
}

void dae::HealthComponent::Heal(int healAmount)
{
	m_Health.current += healAmount;
	if (m_Health.current > m_Health.max)
	{
		m_Health.current = m_Health.max;
	}
}
