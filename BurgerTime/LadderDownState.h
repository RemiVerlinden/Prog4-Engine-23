#pragma once
#include "PlayerState.h"

class LadderDownState final : public PlayerState
{
public:
	virtual ~LadderDownState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
private:

};