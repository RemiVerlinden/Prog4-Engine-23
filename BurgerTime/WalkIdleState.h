#pragma once
#include "PlayerState.h"

// STATE AFTER PLAYER RETURNS TO IDLE AFTER HORINZONTAL WALK
class WalkIdleState final : public PlayerState
{
public:
	virtual ~WalkIdleState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
};