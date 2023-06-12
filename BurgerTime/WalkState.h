#pragma once
#include "PlayerState.h"


// STATE WHEN PLAYER MOVES HORIZONTALLY 
class WalkState final : public PlayerState
{
public:
	virtual ~WalkState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
private:
	void UpdateLookDirection(PSMC* context, float movementX);
};