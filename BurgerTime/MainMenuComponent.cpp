#include "MainMenuComponent.h"
#include "GameObject.h"
#include "Render2DComponent.h"
#include "ResourceManager.h"
#include "Structs.h"

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

		m_StateChanged = false;
	}
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
