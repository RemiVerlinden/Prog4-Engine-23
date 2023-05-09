#pragma once
#include "Event.h"

namespace dae
{
	enum EventType
	{
		OnDeath,
		OnDamage,
		OnGameObjectSpawn,

		EventTypeTotalCount
	};

	class OnDeathEvent : public Event
	{
	public:
		OnDeathEvent(const std::string& objectName) :m_ObjectName(objectName) {}
		std::string GetObjectName() const { return m_ObjectName; }

		EVENT_CLASS_TYPE(OnDeath)
	private:
		std::string m_ObjectName;
	};



	class OnDamageEvent : public Event
	{
	public:
		OnDamageEvent(const std::string& objectName, int newHealth, int damageAmount)
			:m_ObjectName(objectName),m_NewHealth(newHealth),m_DamageAmount(damageAmount){}

		const std::string& GetObjectName() const { return m_ObjectName; }
		int GetCurrentHealth()const { return m_NewHealth; }
		int GetDamageAmount()const { return m_DamageAmount; }

		EVENT_CLASS_TYPE(OnDamage)
	private:
		std::string m_ObjectName;
		int m_NewHealth;
		int m_DamageAmount;
	};



	class OnGameObjectSpawnEvent : public Event
	{
	public:
		OnGameObjectSpawnEvent(const std::string& name) :m_ObjectName(name) {};
		const std::string& GetObjectName() const { return m_ObjectName; }

		EVENT_CLASS_TYPE(OnGameObjectSpawn)
	private:
		std::string m_ObjectName;
	};
}