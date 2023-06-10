#pragma once
#include "GameObject.h"
#include "UpdateContext.h"
#include <iostream>
#include <memory>
#include "../BurgerTime/MoveComponent.h"
#include "SceneManager.h"
#include "../BurgerTime/HealthComponent.h"
#include "../BurgerTime/MainMenuComponent.h"

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


	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, glm::vec2 movedir)
			: GameObjectCommand(gameObject)
			, m_MoveDir(movedir)
			,m_MoveDirRef(nullptr)
			,m_UseRef(false)
		{
			SetMoveComponent(gameObject);
		}

		MoveCommand(GameObject* gameObject, const glm::vec2* movedir)
			: GameObjectCommand(gameObject)
			, m_MoveDir(0,0)
			, m_MoveDirRef(movedir)
			,m_UseRef(true)
		{
			SetMoveComponent(gameObject);
		}

		void Execute(Seconds /*elapsedTime*/)
		{
			glm::vec2 moveVec = (m_UseRef) ? *m_MoveDirRef : m_MoveDir;
			moveVec.y = -moveVec.y; // in this engine the Y is inverted 
			m_MoveComponent->SetMoveDirection(moveVec);
		}
	private:
		MoveComponent* m_MoveComponent;
		
		bool m_UseRef;
		const glm::vec2* m_MoveDirRef;
		glm::vec2 m_MoveDir;

		void SetMoveComponent(GameObject* gameObject)
		{
			if (gameObject->HasComponent<MoveComponent>())
			{
				m_MoveComponent = gameObject->GetComponent<MoveComponent>();
			}
			else
			{
				m_MoveComponent = gameObject->AddComponent<MoveComponent>();
			}
		}
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

	class DamageCommand final : public GameObjectCommand
	{
	public:
		DamageCommand(GameObject* object, int amount) :GameObjectCommand(object), m_DamageAmount(amount) 
		{
			SetHealthComponent(object);
		};
		virtual void Execute(Seconds /*elapsedTime*/)
		{
			m_HealthComponent->Damage(m_DamageAmount);
		};
	private:
		HealthComponent* m_HealthComponent;
		int m_DamageAmount;

		void SetHealthComponent(GameObject* object)
		{
			if (object->HasComponent<HealthComponent>())
			{
				m_HealthComponent = object->GetComponent<HealthComponent>();
			}
			else
			{
				m_HealthComponent = object->AddComponent<HealthComponent>();
			}
		}
	};

	class KillCommand final : public GameObjectCommand
	{
	public:
		KillCommand(GameObject* object) :GameObjectCommand(object){};
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

	class MainMenuSelectDown : public Command
	{
		virtual void Execute(Seconds) override
		{
			int totalstates = static_cast<int>(MainMenuComponent::MainMenuState::TOTALSTATES);
			int currentstate = static_cast<int>(MainMenuComponent::GetMainMenuState());
			int nextState = ++currentstate % totalstates;
			MainMenuComponent::SetMainMenuState(static_cast<MainMenuComponent::MainMenuState>(nextState));
		}
	};

	class MainMenuSelectUp : public Command
	{
		virtual void Execute(Seconds) override
		{
			int totalstates = static_cast<int>(MainMenuComponent::MainMenuState::TOTALSTATES);
			int currentstate = static_cast<int>(MainMenuComponent::GetMainMenuState());
			int nextState = --currentstate;
			if(nextState < 0 ) nextState = totalstates - 1;

			MainMenuComponent::SetMainMenuState(static_cast<MainMenuComponent::MainMenuState>(nextState));
		}
	};

	class MainMenuStartGame : public Command
	{
		virtual void Execute(Seconds) override
		{
			MainMenuComponent::StartGame();
		}
	};
}