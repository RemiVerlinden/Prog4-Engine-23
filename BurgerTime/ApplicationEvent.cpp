#include "ApplicationEvent.h"
#include "GameObject.h"

using namespace engine;

OnTriggerEnterEvent::OnTriggerEnterEvent(GameObject* pTrigger, GameObject* pOther)
	:m_pTrigger(pTrigger)
	, m_pOther(pOther)
{
}

GameObject* engine::OnTriggerEnterEvent::GetTrigger() const
{
	return m_pTrigger;
}

GameObject* engine::OnTriggerEnterEvent::GetOther() const
{
	return m_pOther;
}

engine::OnRespawnEvent::OnRespawnEvent(GameObject* pResurrect)
	:m_pObject(pResurrect)
{
}

GameObject* engine::OnRespawnEvent::GetObject() const
{
	return m_pObject;
}
