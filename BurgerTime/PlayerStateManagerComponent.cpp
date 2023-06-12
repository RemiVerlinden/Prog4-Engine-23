#include "PlayerStateManagerComponent.h"
#include "PlayerState.h"
#include "PlayerStatesInclude.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "PlayerState.h"

PlayerStateManagerComponent::PlayerStateManagerComponent()
	:BaseComponent()
	, m_pCurrentState(nullptr)
{
}

void PlayerStateManagerComponent::Initialize()
{
	EventSubscribe(engine::OnTriggerEnter);

	m_pPlayerComponent = m_GameObject->GetComponent<PlayerComponent>();

	m_pCurrentState = std::make_unique<WalkIdleState>();
	m_pCurrentState->EnterState(this);
}

void PlayerStateManagerComponent::LateUpdate(const engine::UpdateContext& /*context*/)
{
	m_pCurrentState->Update(this);
}

void PlayerStateManagerComponent::OnEvent(const engine::Event& sent)
{
	if (sent.GetEventType() == engine::OnTriggerEnter)
	{
		m_pCurrentState->OnTriggerEnter(this, static_cast<const engine::OnTriggerEnterEvent&>(sent));
	}
}

PlayerComponent* PlayerStateManagerComponent::GetPlayerComponent() const
{
	return m_pPlayerComponent;
}

void PlayerStateManagerComponent::SwitchState(std::unique_ptr<PlayerState>&& state)
{
	m_pCurrentState = std::move(state);
	m_pCurrentState->EnterState(this);
}

PlayerStateEnum PlayerStateManagerComponent::GetState() const
{
	return m_pCurrentState->GetCurrentState();
}
