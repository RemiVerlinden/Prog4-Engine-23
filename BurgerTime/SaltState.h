#pragma once
#include "PlayerState.h"

// WHEN THROWING SALT
class SaltState final : public PlayerState
{
public:
	virtual ~SaltState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
};
