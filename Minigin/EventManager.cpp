#include "EventManager.h"
#include <algorithm>
using namespace dae;

// EventManager function definitions
void EventManager::Update()
{

}

void EventManager::SendEvent(const Event& sent)
{
    for (EventListener* pListener : m_Listeners)
    {
        auto& subscriptions = pListener->m_Subscriptions;
        EventType type = sent.GetEventType();

        if (subscriptions.find(type) != subscriptions.end())
        {
            pListener->OnEvent(sent);
        }
    }
}

void EventManager::AddSubscriber(EventListener* pListener)
{
    m_Listeners.insert(pListener);
}

void EventManager::RemoveSubscriber(EventListener* pListener)
{
    auto it = m_Listeners.find(pListener);
    if (it != m_Listeners.end())
    {
        m_Listeners.erase(it);
    }
}
