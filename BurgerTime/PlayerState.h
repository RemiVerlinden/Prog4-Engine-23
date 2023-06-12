#pragma once
#include "PlayerStateManagerComponent.h"
#include "PlayerStateEnum.h"

//#define DEBUG_PLAYER_STATES

// BASE STATE FOR ALL PLAYER ACTIVITIES
namespace engine 
{
	class AnimatedSpriteComponent;
}

class PlayerState
{
public:
	using PSMC = PlayerStateManagerComponent;


	virtual ~PlayerState() = default;
	virtual void EnterState(PSMC* context) = 0;
	virtual void Update(PSMC* context) = 0;
	virtual void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& sent) = 0;
	PlayerStateEnum GetCurrentState() { return m_PlayerState; }
protected:
	void CheckForTrigger(PSMC* context, const engine::OnTriggerEnterEvent& sent);
	void SetPlayerSpriteAnimation(PSMC* context, int startSpriteID, int endSpriteID) const;

	PlayerStateEnum m_PlayerState;
	struct Settings // percent of speed needed to change state
	{
		static inline const float idleToWalk		= 0.05f;
		static inline const float idleToLadder		= 0.05f;
		static inline const float WalkToLadder		= 0.15f;
		static inline const float LadderToLadder	= 0.2f;
		static inline const float ladderToWalk		= 0.995f;
		static inline const float anythingToIdle	= 0.05f;
		static inline const float buggedWalk		= 0.5f;
	};
};


