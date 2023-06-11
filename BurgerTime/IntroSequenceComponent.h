#pragma once
#include "BaseComponent.h"
#include <array>
#include "Timers.h"

class engine::GameObject;
class IntroSequenceComponent : public engine::BaseComponent
{
public:
	virtual ~IntroSequenceComponent();

	void Initialize() override;
	void Update(const engine::UpdateContext& context) override;
	void OnSceneActivate() override;
private:
	void InitSequence1();
	void InitSequence2();
	void InitSequence3();
	void InitMainMenuFinal();

	void SetIntroSequenceActive(int sequenceID);
	std::array<engine::GameObject*, 3> m_IntroObjects{nullptr};
	int m_ActiveSequence = 0;
	const int m_TotalSequences = 2;
	static inline bool m_IsComponentAlive = false;

	engine::ManualCountdownTimer m_Timer{};
	engine::Seconds m_SequenceDuration{ 3.0f };
};
