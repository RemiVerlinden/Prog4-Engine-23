#include "WalkState.h"
#include "AppComponents.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void WalkState::EnterState(PSMC* context)
{
	m_PlayerState = PlayerStateEnum::Walk;

	SetPlayerSpriteAnimation(context, 3, 5);
#ifdef DEBUG_PLAYER_STATES
	APP_INFO("Entered Walking State!");
#endif
}

void WalkState::Update(PSMC* context)
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();

	const glm::vec2& moveDelta = pPlayerComponent->GetMoveDelta();

	float moveSpeed = PlayerComponent::Settings::Speed;
	float moveX = moveDelta.x;
	float moveY = moveDelta.y;

	UpdateLookDirection(context, moveX);

	if (moveY > moveSpeed * Settings::WalkToLadder)
	{
		context->SwitchState(std::make_unique<LadderUpState>());
	}
	else if (moveY < -moveSpeed * Settings::WalkToLadder)
	{
		context->SwitchState(std::make_unique<LadderDownState>());
	}
	else if (abs(moveX) < moveSpeed * Settings::anythingToIdle)
	{
		context->SwitchState(std::make_unique<WalkIdleState>());
	}
}

void WalkState::OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent)
{
	CheckForTrigger(context, sent);
}

void WalkState::UpdateLookDirection(PSMC* context, float movementX )
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();
	AnimatedSpriteComponent* pSpriteComponent = pPlayerComponent->GetAnimatedSpriteComponent();

	bool movingRight = movementX > 0.f;
	engine::render::FlipTexture flip = movingRight ? engine::render::SDL_FLIP_HORIZONTAL : engine::render::SDL_FLIP_NONE;

	pSpriteComponent->SetFlipTexture(flip);
}
