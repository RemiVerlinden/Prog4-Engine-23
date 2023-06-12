#include "LadderDownIdleState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void LadderDownIdleState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::LadderDownIdle;

	SetPlayerSpriteAnimation(context, 1, 1);

#ifdef DEBUG_PLAYER_STATES

	APP_INFO("Ladder DOWN IDLE State!");
#endif
}

void LadderDownIdleState::Update(PSMC*context )
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();

	const glm::vec2& moveDelta = pPlayerComponent->GetMoveDelta();

	float moveSpeed = PlayerComponent::Settings::Speed;
	float moveX = moveDelta.x;
	float moveY = moveDelta.y;


	if (moveY < -moveSpeed * Settings::idleToLadder)
	{
		context->SwitchState(std::make_unique<LadderDownState>());
	}
	else if (moveY > moveSpeed * Settings::LadderToLadder)
	{
		context->SwitchState(std::make_unique<LadderUpState>());
	}
	else if ((abs(moveX) > moveSpeed * Settings::ladderToWalk))
	{
		context->SwitchState(std::make_unique<WalkState>());
	}
}

void LadderDownIdleState::OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent )
{
	CheckForTrigger(context, sent);
}

// ladderdown ladderup walk