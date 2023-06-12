#include "DeadState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"
#include "EventManager.h"

using namespace engine;


void DeadState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::Dead;

	SetPlayerSpriteAnimation(context, 18, 23);

	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();
	AnimatedSpriteComponent* pSpriteComponent = pPlayerComponent->GetAnimatedSpriteComponent();

	pSpriteComponent->SetPlayOnce(true);

	Locator::GetSoundSystem().Play("die.wav", 1.f, true);
	m_OriginalFrametime = pSpriteComponent->GetFrameTime();
	pSpriteComponent->SetFrameTime(m_DeathFrametime);
}

void DeadState::Update(PSMC* context)
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();
	AnimatedSpriteComponent* pSpriteComponent = pPlayerComponent->GetAnimatedSpriteComponent();

	if (pSpriteComponent->IsPaused() && !m_IsDead)
	{
		m_IsDead = true;
		pSpriteComponent->SetFrameTime(m_OriginalFrametime);
		EventManager::GetInstance().SendEvent(OnDeathEvent{context->GetGameObject()->GetTag()});
	}
}

void DeadState::OnTriggerEnter(PSMC* , const engine::OnTriggerEnterEvent& )
{
}
