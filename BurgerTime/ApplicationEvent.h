#pragma once
#include "Event.h"

namespace engine
{
	enum EventType
	{
		OnDeath,
		OnGameObjectSpawn,
		OnTriggerEnter,
		OnRespawn,
		OnGameOver,
		OnPickup,

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

	class OnGameObjectSpawnEvent : public Event
	{
	public:
		OnGameObjectSpawnEvent(const std::string& name) :m_ObjectName(name) {};
		const std::string& GetObjectName() const { return m_ObjectName; }

		EVENT_CLASS_TYPE(OnGameObjectSpawn)
	private:
		std::string m_ObjectName;
	};

	class GameObject;
	class OnTriggerEnterEvent : public Event
	{
	public:
		OnTriggerEnterEvent(GameObject* pTrigger, GameObject* pOther);
		GameObject* GetTrigger() const;
		GameObject* GetOther() const;

		EVENT_CLASS_TYPE(OnTriggerEnter)
	private:
		GameObject* m_pTrigger;
		GameObject* m_pOther;
	};

	class OnRespawnEvent : public Event
	{
	public:
		OnRespawnEvent(GameObject* pResurrect);
		GameObject* GetObject() const;

		EVENT_CLASS_TYPE(OnRespawn)
	private:
		GameObject* m_pObject;

	};

	class OnGameOverEvent : public Event
	{
	public:
		OnGameOverEvent(const std::string& objectName) :m_ObjectName(objectName) {}
		std::string GetObjectName() const { return m_ObjectName; }

		EVENT_CLASS_TYPE(OnGameOver)
	private:
		std::string m_ObjectName;

	};

	class OnPickupEvent : public Event
	{
	public:
		OnPickupEvent(const std::string& objectName, std::string item) :m_ObjectName(objectName), m_ItemName(item) {}
		std::string GetObjectName() const { return m_ObjectName; }
		std::string GetPickupName() const { return m_ObjectName; }

		EVENT_CLASS_TYPE(OnPickup)
	private:
		std::string m_ObjectName;
		std::string m_ItemName;
	};
}