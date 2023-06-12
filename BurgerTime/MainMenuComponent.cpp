#include "MainMenuComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Structs.h"
#include "Locator.h"
#include "Common.h"
#include "Gamemode.h"

void MainMenuComponent::OnEvent(const engine::Event& /*sent*/)
{
}

void MainMenuComponent::Initialize()
{
	int index = 0;
	auto font = engine::ResourceManager::GetInstance().LoadFont("fonts/burger-time.otf", static_cast<unsigned int>(14));

	for (const std::string& menuOption : m_MainMenuOptions)
	{
		float middleX = engine::WindowSettings::defaultwidth / 2;
		engine::TextComponent* textComp = m_GameObject->AddComponent<engine::TextComponent>(menuOption);
		textComp->SetText(menuOption);
		textComp->SetFont(font);

		// we want to have all the text centered
		int letters = static_cast<int>(menuOption.size());
		float drawX = middleX - (letters/2) * 14;
		textComp->SetPosition(drawX, 100.f + (index * 27));
		m_MenuTextComponents[index] = textComp;

		++index;
	}
	SetMainMenuState(MainMenuState::SINGLEPLAYER);
}

void MainMenuComponent::Update(const engine::UpdateContext& /*context*/)
{
	using namespace engine;

	if (m_StartGame)
	{
		std::cout << "starting game";
		m_StartGame = false;
	}

	if (m_StateChanged)
	{
		for (engine::TextComponent* textComp : m_MenuTextComponents)
			textComp->SetColor(255,255,255,255 );

		m_MenuTextComponents[static_cast<int>(m_MainMenuState)]->SetColor(255, 255, 4,255);

		Locator::GetSoundSystem().Play("menu_move.wav", 1.f, true);

		m_StateChanged = false;
	}
}

void MainMenuComponent::OnSceneActivate()
{
	using namespace engine;
	std::string soundPath = "burgertimemenu.wav";
		Locator::GetSoundSystem().Play(soundPath, 0.3f, false);
}

void MainMenuComponent::OnSceneDeactivate()
{
	using namespace engine;
	std::string soundPath = "menu_select.wav";
	Locator::GetSoundSystem().Play(soundPath, 1.f, true);

	std::string musicPath = "main.wav";
	Locator::GetSoundSystem().Play(musicPath, 1.f, false);

	Gamemode::GameType gamemode = Gamemode::GameType::SinglePlayer;
	switch (m_MainMenuState)
	{
		case MainMenuComponent::MainMenuState::SINGLEPLAYER:
			gamemode = Gamemode::GameType::SinglePlayer;
			break;
		case MainMenuComponent::MainMenuState::COOP:
			gamemode = Gamemode::GameType::Coop;
			break;
		case MainMenuComponent::MainMenuState::VERSUS:
			gamemode = Gamemode::GameType::Versus;
			break;
	}

	Gamemode::GetInstance().SetGamemode(gamemode);
}

void MainMenuComponent::SetMainMenuState(MainMenuState state)
{
	m_StateChanged = true;
	m_MainMenuState = state;
}

void MainMenuComponent::StartGame()
{
	m_StartGame = true;
}
