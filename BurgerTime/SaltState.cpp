#include "SaltState.h"
#include "AppComponents.h"

using namespace engine;

void SaltState::EnterState(PSMC* )
{
	m_PlayerState = PlayerStateEnum::Salt;

}

void SaltState::Update(PSMC* )
{
}

void SaltState::OnTriggerEnter(PSMC* , const engine::OnTriggerEnterEvent& )
{
}
