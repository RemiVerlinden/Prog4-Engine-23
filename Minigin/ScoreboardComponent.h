#pragma once
#include "BaseComponent.h"
#include "EventListener.h"
#include <memory>
#include "font.h"
#include <format>
namespace dae
{
	class TextComponent;
	class ScoreBoardComponent final : public BaseComponent, public EventListener
	{
	public:
		void Initialize() override;
		void DrawUI(UpdateContext& context) override;
		virtual void OnEvent(const Event& sent);
	private:
		void PlayerDied(int playerID);
		void PlayerDamaged(int playerID, int newHealth);
		struct PlayerInfo
		{
			int lives = 3;
			int health = 100;
			int score = 0;
			std::string GetLivesString() { return std::format("{} Lives", lives); }
			std::string GetHealthString() { return std::format("{} Health", health); }
			std::string GetScoreString() { return std::format("Score: {}", score); }
		};
		struct PlayerHUD
		{
			PlayerInfo info;
			TextComponent* m_LivesText;
			TextComponent* m_HealthText;
			TextComponent* m_ScoreText;
		};

		std::vector <PlayerHUD> m_PlayerVec;

		std::shared_ptr<Font> m_Font;


	private:
		void Clone(GameObject* clone) override;
	}; 

}
