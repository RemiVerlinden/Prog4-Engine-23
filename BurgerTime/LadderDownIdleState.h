#pragma once
#include "PlayerState.h"

class LadderDownIdleState : public PlayerState
{
public:
	virtual ~LadderDownIdleState() = default;
	
	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
private:

};
