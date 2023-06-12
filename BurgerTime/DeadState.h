#pragma once
#include "PlayerState.h"


// hmm what could this be?
class DeadState final : public PlayerState
{
public:

	virtual ~DeadState() = default;

	void EnterState(PSMC* context) override;
	void Update(PSMC* context) override;
	void OnTriggerEnter(PSMC* context, const engine::OnTriggerEnterEvent& event) override;
private:
	bool m_IsDead = false;
	float m_OriginalFrametime = 0.f;
	float m_DeathFrametime = 0.3f;
};
