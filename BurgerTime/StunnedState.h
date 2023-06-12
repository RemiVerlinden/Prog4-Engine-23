#pragma once
#include "PlayerState.h"

// STUN STATE 
class StunnedState : public PlayerState
{
public:
	virtual ~StunnedState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
private:

};