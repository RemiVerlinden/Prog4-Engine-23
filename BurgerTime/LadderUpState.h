#pragma once
#include "PlayerState.h"

// STATE WHEN VERTICALLY MOVING, WHEN CLIMBING LADDER
class LadderUpState final : public PlayerState
{
public:
	virtual ~LadderUpState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
};


