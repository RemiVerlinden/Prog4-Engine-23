#include "StunnedState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void StunnedState::EnterState(PSMC* )
{
	m_PlayerState = PlayerStateEnum::Stunned;
}

void StunnedState::Update(PSMC* )
{
}

void StunnedState::OnTriggerEnter(PSMC* , const engine::OnTriggerEnterEvent& )
{
}
