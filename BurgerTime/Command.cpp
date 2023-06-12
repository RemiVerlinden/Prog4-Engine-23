#include "Command.h"
#include "AppComponents.h"
#include "SceneFactory.h"
#include "Gamemode.h"

engine::MoveCommand::MoveCommand(PlayerComponent* pPlayerComponent, glm::vec2 movedir)
	: m_MoveDir(movedir)
	, m_pPlayerComponent(pPlayerComponent)
	, m_MoveDirRef(nullptr)
	, m_UseRef(false)
{
}

void engine::MoveCommand::Execute(Seconds)
{
	glm::vec2 moveVec = (m_UseRef) ? *m_MoveDirRef : m_MoveDir;
	moveVec.y = -moveVec.y; // in this engine the Y is inverted 
	m_pPlayerComponent->SetMoveDirection(moveVec);
}

// I dont use this command anymore
void engine::StartGameCommand::Execute(Seconds)
{
	//SceneManager& sceneManager = SceneManager::GetInstance();

	//sceneManager.SetActiveGameScene(SceneFactory::InitBurgerTimeStage("stage1", true));
	//SceneFactory::InitBurgerTimeStage("stage2", false);
	//SceneFactory::InitBurgerTimeStage("stage3", true);
}

void engine::MainMenuStartGame::Execute(Seconds)
{
	Scene* currentScene = SceneManager::GetInstance().GetActiveGameScene();
	if (m_pScene != currentScene) return;
	//MainMenuComponent::StartGame();
	Gamemode::GameType gamemode = Gamemode::GameType::SinglePlayer;

	MainMenuComponent::MainMenuState state = MainMenuComponent::GetMainMenuState();
	switch (state)
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


	SceneFactory::InitBurgerTimeStage("stage1", true);
	SceneFactory::InitBurgerTimeStage("stage2", false);
	SceneFactory::InitBurgerTimeStage("stage3", false);
}
