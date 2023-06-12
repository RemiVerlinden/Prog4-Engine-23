#include "ScoreboardComponent.h"
#include "ImguiWrapper.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "EventManager.h"

void engine::ScoreBoardComponent::Initialize()
{
	EventSubscribe(OnDeath);

	m_Font = engine::ResourceManager::GetInstance().LoadFont("fonts/lowres.ttf", 20);

	size_t hardCodedTotalPlayers = 2;
	glm::vec2 textPos{0,300};
	for (size_t i = 0; i < hardCodedTotalPlayers; i++)
	{
		textPos.y += i * 30;
		textPos.x = 0;

		PlayerHUD playerHUD;
		playerHUD.info.lives = 3;
		playerHUD.info.health = 100;
		playerHUD.info.score = 0;

		playerHUD.m_LivesText = m_GameObject->AddComponent<TextComponent>(std::format("Player{}_LivesText", i));
		playerHUD.m_LivesText->SetText(playerHUD.info.GetLivesString());
		playerHUD.m_LivesText->SetFont(m_Font);
		playerHUD.m_LivesText->SetPosition(textPos);
		textPos.x += 100;

		playerHUD.m_HealthText = m_GameObject->AddComponent<TextComponent>(std::format("Player{}_HealthText", i));
		playerHUD.m_HealthText->SetText(playerHUD.info.GetHealthString());
		playerHUD.m_HealthText->SetFont(m_Font);
		playerHUD.m_HealthText->SetPosition(textPos);
		textPos.x += 100;

		playerHUD.m_ScoreText = m_GameObject->AddComponent<TextComponent>(std::format("Player{}_ScoreText", i));
		playerHUD.m_ScoreText->SetText(playerHUD.info.GetScoreString());
		playerHUD.m_ScoreText->SetFont(m_Font);
		playerHUD.m_ScoreText->SetPosition(textPos);

		m_PlayerVec.push_back(playerHUD);
	}
}


void engine::ScoreBoardComponent::DrawUI(UpdateContext& /*context*/)
{
	ImGui::Begin("Scoreboard");

	ImGui::Text("Player 1 Lives %d", m_PlayerVec[0].info.lives);

	ImGui::End();
}

void engine::ScoreBoardComponent::OnEvent(const Event& sent)
{
	switch (sent.GetEventType())
	{
		case EventType::OnDeath:
		{
			const OnDeathEvent& event = reinterpret_cast<const OnDeathEvent&>(sent);
			if (event.GetObjectName() == "Player1")
			{
				PlayerDied(event.GetObjectName(), 0);
			}
			else if (event.GetObjectName() == "Player2")
			{
				PlayerDied(event.GetObjectName(), 1);
			}
			break;
		}

	}
}

void engine::ScoreBoardComponent::PlayerDied(std::string player, int playerID)
{
	PlayerHUD& playerHUD = m_PlayerVec[playerID];
	PlayerInfo& playerInfo = m_PlayerVec[playerID].info;

	--playerInfo.lives;

	playerHUD.m_LivesText->SetText(playerInfo.GetLivesString());

	if (playerInfo.lives > 0 )
	{
		OnGameObjectSpawnEvent resurrectEvent{ player };
		EventManager::GetInstance().SendEvent(resurrectEvent);
	}
}

void engine::ScoreBoardComponent::PlayerDamaged(int playerID, int newHealth)
{
	PlayerHUD& playerHUD = m_PlayerVec[playerID];
	playerHUD.info.health = newHealth;
	playerHUD.m_HealthText->SetText(playerHUD.info.GetHealthString());
}

