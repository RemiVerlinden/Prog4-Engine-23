#pragma once
#include "Singleton.h"
#include "EventListener.h"
#include <unordered_set>
#include <vector>
#include <iostream>
namespace engine
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		void Update();
		void SendEvent(const Event& sent);
	private:
		friend class Singleton<EventManager>;
		EventManager() = default;
		friend class EventListener;
		void AddSubscriber(EventListener* pListener);
		void RemoveSubscriber(EventListener* pListener);

		using UniqueListeners = std::unordered_set<EventListener*>;
		UniqueListeners m_Listeners;
	};
}
