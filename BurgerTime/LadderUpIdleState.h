#pragma once
#include "PlayerState.h"

// STATE AFTER STOPPED CLIMBING LADDER
class LadderUpIdleState final : public PlayerState
{
public:
	virtual ~LadderUpIdleState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
};

