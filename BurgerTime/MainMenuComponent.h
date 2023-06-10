#pragma once
#include "BaseComponent.h"
#include "EventListener.h"
#include <array>
#include "TextComponent.h"


class MainMenuComponent final : public engine::BaseComponent, public engine::EventListener
{
public:
	MainMenuComponent() = default;
	virtual ~MainMenuComponent() = default;

	virtual void OnEvent(const engine::Event& sent) override;

	virtual void Initialize() override;
	virtual void Update(const engine::UpdateContext& context) override;
	virtual void OnSceneActivate() override;
	virtual void OnSceneDeactivate() override;

	enum class MainMenuState
	{
		SINGLEPLAYER,
		MULIPLAYER,
		VERSUS,
		TOTALSTATES
	};

	static MainMenuState GetMainMenuState() { return m_MainMenuState; }
	static void SetMainMenuState(MainMenuState state);
	static void StartGame();
protected:
private:
	static inline MainMenuState m_MainMenuState;
	static inline bool m_StateChanged = false;
	static inline bool m_StartGame = false;
	std::array<std::string,3> m_MainMenuOptions{ "SINGLEPLAYER","MULTIPLAYER","VERSUS" };
	std::array<engine::TextComponent*, 3> m_MenuTextComponents{nullptr, nullptr, nullptr};
};
