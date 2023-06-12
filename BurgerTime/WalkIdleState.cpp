#include "WalkIdleState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void WalkIdleState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::WalkIdle;
	SetPlayerSpriteAnimation(context, 1, 1);
#ifdef DEBUG_PLAYER_STATES
	APP_INFO("Entered WALK Idle State!");
#endif
}

void WalkIdleState::Update(PSMC* context)
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();

	const glm::vec2& moveDelta = pPlayerComponent->GetMoveDelta();

	float moveSpeed = PlayerComponent::Settings::Speed;
	float moveX = moveDelta.x;
	float moveY = moveDelta.y;

	if (moveY > moveSpeed * Settings::idleToLadder)
	{
		context->SwitchState(std::make_unique<LadderUpState>());
	}
	else if (moveY < -moveSpeed * Settings::idleToLadder)
	{
		context->SwitchState(std::make_unique<LadderDownState>());
	}
	else if(abs(moveX) > moveSpeed * Settings::idleToWalk)
	{
		context->SwitchState(std::make_unique<WalkState>());
	}
}

void WalkIdleState::OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent)
{
	CheckForTrigger(context, sent);
}