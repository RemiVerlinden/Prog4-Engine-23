#include "EventListener.h"
#include "EventManager.h"
#include <iostream>
using namespace engine;

EventListener::EventListener()
{
    EventManager::GetInstance().AddSubscriber(this);

}

EventListener::~EventListener()
{
    EventManager::GetInstance().RemoveSubscriber(this);
}

void EventListener::EventSubscribe(EventType event)
{
    m_Subscriptions.insert(event);
}

void EventListener::EventUnsubscribe(EventType event)
{
    m_Subscriptions.erase(event);
}

void EventListener::EventUnsubscribeAll()
{
    m_Subscriptions.clear();
}

