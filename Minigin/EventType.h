#pragma once
#include <string>
namespace dae
{
	enum EventType
	{
		OnDeath,
		OnDamage,
		PlayerResurrect,

		EventTypeTotalCount
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
	};

	class OnDeathEvent : public Event
	{
		EVENT_CLASS_TYPE(OnDeath)

	public:
		OnDeathEvent(const std::string& objectName) :m_ObjectName(objectName) {}
		std::string GetObjectName() const { return m_ObjectName; }
	private:
		std::string m_ObjectName;
	};

	class OnDamageEvent : public Event
	{
		EVENT_CLASS_TYPE(OnDamage)

	public:
		OnDamageEvent(const std::string& objectName, int newHealth, int damageAmount)
		:m_ObjectName(objectName)
		,m_NewHealth(newHealth)
		,m_DamageAmount(damageAmount)
		{}

		std::string GetObjectName() const { return m_ObjectName; }
		int GetCurrentHealth()const { return m_NewHealth; }
		int GetDamageAmount()const { return m_DamageAmount; }
	private:
		std::string m_ObjectName;
		int m_NewHealth;
		int m_DamageAmount;
	};

	class PlayerResurrectEvent : public Event
	{
		EVENT_CLASS_TYPE(PlayerResurrect)

	public:
		PlayerResurrectEvent(int playerID, int lives) :m_PlayerID(playerID), m_RemainingLives(lives) {};
		int GetPlayerID() { return m_PlayerID; }
		int GetRemainingLives() { return m_RemainingLives; }
	private:
		int m_PlayerID;
		int m_RemainingLives;
	};
}