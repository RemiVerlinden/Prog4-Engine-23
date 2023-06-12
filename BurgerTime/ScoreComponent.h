#pragma once
#include "BaseComponent.h"
#include "EventListener.h"
#include "glm\glm.hpp"
#include "Timers.h"
namespace engine 
{
	class TextComponent;
}

class ScoreComponent final : public engine::BaseComponent, public engine::EventListener
{
public:
	virtual ~ScoreComponent() = default;
	virtual void OnEvent(const engine::Event& sent) override;
	virtual void Initialize() override;
private:
	engine::TextComponent* InitTextComponent(glm::ivec4 color = glm::ivec4{ 255,255,255,255 });
	void InitSinglePlayer();
	void InitCoop();
	void InitVersus();
	virtual void OnSceneDeactivate();
	virtual void Update(const engine::UpdateContext& context) override;

	int m_ScorePlayer1 = 0;
	int m_ScorePlayer2 = 0;
	engine::TextComponent* m_pPlayer1ScoreText = nullptr;
	engine::TextComponent* m_pPlayer2ScoreText = nullptr;

	engine::ManualCyclicTimer m_Timer;
};