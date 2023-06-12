#pragma once
#include "GameObject.h"
#include "UpdateContext.h"
#include <iostream>
#include <memory>
#include "../BurgerTime/MoveComponent.h"
#include "SceneManager.h"
#include "../BurgerTime/MainMenuComponent.h"

class PlayerComponent;

namespace engine
{
	class Command
	{
	public:
		virtual void Execute(Seconds elapsedTime) = 0;
		virtual ~Command() = default;
	};


	class GameObjectCommand : public Command
	{
	public:
		virtual void Execute(Seconds elapsedTime) = 0;
		virtual ~GameObjectCommand() = default;
	protected:
		GameObjectCommand(GameObject* gameObject) : m_GameObject{ gameObject } {}
		GameObject* GetGameObject() { return m_GameObject; }
	private:
		GameObject* m_GameObject;
	};


	class MoveCommand final : public Command
	{
	public:
		MoveCommand(PlayerComponent* pPlayerComponent, glm::vec2 movedir);
		virtual ~MoveCommand() = default;

		virtual void Execute(Seconds) override;

	private:
		PlayerComponent* m_pPlayerComponent;

		bool m_UseRef;
		const glm::vec2* m_MoveDirRef;
		glm::vec2 m_MoveDir;
	};

	class ChangeSceneCommand final : public Command
	{
	public:
		ChangeSceneCommand(bool nextScene) :m_NextScene(nextScene) {}

		virtual void Execute(Seconds /*elapsedTime*/)
		{
			SceneManager& sceneManager = SceneManager::GetInstance();

			(m_NextScene) ? sceneManager.NextScene() : sceneManager.PreviousScene();
		}

	private:
		bool m_NextScene;
	};

	class KillCommand final : public GameObjectCommand
	{
	public:
		KillCommand(GameObject* object) :GameObjectCommand(object) {};
		virtual void Execute(Seconds /*elapsedTime*/)
		{
			GetGameObject()->Destroy();
		};
	private:
	};

	class StartSingleplayerCommand : public Command
	{
		virtual void Execute(Seconds) override
		{
			SceneManager& sceneManager = SceneManager::GetInstance();

			sceneManager.SetActiveGameScene("stage1_singleplayer");
		}
	};

	class StartMultiplayerCommand : public Command
	{
		virtual void Execute(Seconds) override
		{
			SceneManager& sceneManager = SceneManager::GetInstance();

			sceneManager.SetActiveGameScene("stage1_multiplayer");
		}
	};

	class StartVersusCommand : public Command
	{
		virtual void Execute(Seconds) override
		{
			SceneManager& sceneManager = SceneManager::GetInstance();

			sceneManager.SetActiveGameScene("stage1_multiplayer");
		}
	};

	class StartGameCommand : public Command
	{
		virtual void Execute(Seconds) override;
	};

	class MainMenuSelectDown : public Command
	{
		public:
			MainMenuSelectDown(Scene* pScene) : m_pScene(pScene) {}
	private:
		virtual void Execute(Seconds) override
		{
			Scene* currentScene = SceneManager::GetInstance().GetActiveGameScene();
			if (m_pScene != currentScene) return;

			int totalstates = static_cast<int>(MainMenuComponent::MainMenuState::TOTALSTATES);
			int currentstate = static_cast<int>(MainMenuComponent::GetMainMenuState());
			int nextState = ++currentstate % totalstates;
			MainMenuComponent::SetMainMenuState(static_cast<MainMenuComponent::MainMenuState>(nextState));
		}
		Scene* m_pScene;
	};

	class MainMenuSelectUp : public Command
	{
	public:
		MainMenuSelectUp(Scene* pScene) : m_pScene(pScene) {}
	private:
		virtual void Execute(Seconds) override
		{
			Scene* currentScene = SceneManager::GetInstance().GetActiveGameScene();
			if (m_pScene != currentScene) return;

			int totalstates = static_cast<int>(MainMenuComponent::MainMenuState::TOTALSTATES);
			int currentstate = static_cast<int>(MainMenuComponent::GetMainMenuState());
			int nextState = --currentstate;
			if (nextState < 0) nextState = totalstates - 1;

			MainMenuComponent::SetMainMenuState(static_cast<MainMenuComponent::MainMenuState>(nextState));
		}
		Scene* m_pScene;
	};

	class MainMenuStartGame : public Command
	{
	public:
		MainMenuStartGame(Scene* pScene) : m_pScene( pScene ) {}
	private:
		virtual void Execute(Seconds) override;
		Scene* m_pScene;
	};

}