#include "LadderDownState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;


void LadderDownState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::LadderDown;

	SetPlayerSpriteAnimation(context, 0, 2);

#ifdef DEBUG_PLAYER_STATES
	APP_INFO("Ladder DOWN State!");
#endif
}

void LadderDownState::Update(PSMC* context)
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();

	const glm::vec2& moveDelta = pPlayerComponent->GetMoveDelta();

	float moveSpeed = PlayerComponent::Settings::Speed;
	float moveX = moveDelta.x;
	float moveY = moveDelta.y;


	if (moveY > moveSpeed * Settings::LadderToLadder)
	{
		context->SwitchState(std::make_unique<LadderUpState>());
	}
	else if (abs(moveY) < moveSpeed * Settings::anythingToIdle && abs(moveX) < moveSpeed * Settings::anythingToIdle)
	{
		context->SwitchState(std::make_unique<LadderDownIdleState>());
	}
	 else if (abs(moveX) > moveSpeed * Settings::ladderToWalk)
	{
		context->SwitchState(std::make_unique<WalkState>());
	}
	 else if (abs(moveX) > moveSpeed * Settings::buggedWalk && abs(moveY) < moveSpeed * 0.001f)
	{
		context->SwitchState(std::make_unique<WalkState>());
	}
}

void LadderDownState::OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent)
{
	CheckForTrigger(context, sent);
}

// ladderUp, ladderdownIdle, walk