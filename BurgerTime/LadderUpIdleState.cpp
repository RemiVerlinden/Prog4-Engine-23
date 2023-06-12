#include "LadderUpIdleState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void LadderUpIdleState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::LadderUpIdle;

	SetPlayerSpriteAnimation(context, 7, 7);

#ifdef DEBUG_PLAYER_STATES
	APP_INFO("Entered Ladder UP IDLE State!");
#endif

}

void LadderUpIdleState::Update(PSMC* context)
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
	 else if (moveY < -moveSpeed * Settings::LadderToLadder)
	{
		context->SwitchState(std::make_unique<LadderDownState>());
	}
	else if ((abs(moveX) > moveSpeed * Settings::ladderToWalk))
	{
		context->SwitchState(std::make_unique<WalkState>());
	}
}

void LadderUpIdleState::OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent)
{
	CheckForTrigger(context, sent);
}
