#include "LadderUpState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;


void LadderUpState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::LadderUp;

	SetPlayerSpriteAnimation(context, 6, 8);

#ifdef DEBUG_PLAYER_STATES
	APP_INFO("Entered Ladder UP State!");
#endif

}

void LadderUpState::Update(PSMC* context)
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();

	const glm::vec2& moveDelta = pPlayerComponent->GetMoveDelta();

	float moveSpeed = PlayerComponent::Settings::Speed;
	float moveX = moveDelta.x;
	float moveY = moveDelta.y;


	 if (moveY < -moveSpeed * Settings::LadderToLadder)
	{
		context->SwitchState(std::make_unique<LadderDownState>());
	}
	else if (abs(moveY) < moveSpeed * Settings::anythingToIdle && abs(moveX) < moveSpeed * Settings::anythingToIdle)
	{
		context->SwitchState(std::make_unique<LadderUpIdleState>());
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

void LadderUpState::OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent)
{
	CheckForTrigger(context, sent);
}