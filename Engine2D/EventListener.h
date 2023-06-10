#pragma once
#include <unordered_set>
#include "../BurgerTime/ApplicationEvent.h"
namespace engine
{
	class EventListener
	{
	public:
		virtual void OnEvent(const Event& sent) = 0;
		void EventSubscribe(EventType event);
		void EventUnsubscribe(EventType event);
		void EventUnsubscribeAll();

		EventListener();
		virtual ~EventListener();

	private:
		friend class EventManager;

		using UniqueEvents = std::unordered_set<EventType>;
		UniqueEvents m_Subscriptions;
	};
}
