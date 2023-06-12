#include "PlayerState.h"
#include "AppComponents.h"
#include "glm\glm.hpp"
#include "GameObject.h"
#include "Locator.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void PlayerState::CheckForTrigger(PSMC* context, const engine::OnTriggerEnterEvent& sent)
{
	engine::GameObject* pOther = sent.GetOther();
	if (pOther->HasComponent<EnemyComponent>())
	{
		context->SwitchState(std::make_unique<DeadState>());
	}
	else if (pOther->HasComponent<SaltComponent>())
	{
		context->SwitchState(std::make_unique<SaltState>());
	}
}

void PlayerState::SetPlayerSpriteAnimation(PSMC* context, int startSpriteID, int endSpriteID) const
{
	PlayerComponent* pPlayerComponent = context->GetPlayerComponent();
	AnimatedSpriteComponent* pSpriteComponent = pPlayerComponent->GetAnimatedSpriteComponent();

	pSpriteComponent->SetFrameRange(startSpriteID, endSpriteID);
}
