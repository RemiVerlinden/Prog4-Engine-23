#pragma once
#include "GameObject.h"
#include "UpdateContext.h"
#include <iostream>
#include <memory>
#include "MoveComponent.h"

namespace dae
{
	class Command
	{
	public:
		virtual void Execute(Seconds elapsedTime) = 0;
		virtual ~Command() = default;
	protected:
		Command(GameObject* gameObject) : m_GameObject{ gameObject } {}
		GameObject* GetActor() { return m_GameObject; }
	private:
		GameObject* m_GameObject;
	};

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* gameObject, glm::vec2 movedir)
			: Command(gameObject)
			, m_MoveDir(movedir)
			,m_MoveDirRef(nullptr)
			,useRef(false)
		{
			SetMoveComponent(gameObject);
		}

		MoveCommand(GameObject* gameObject, const glm::vec2* movedir)
			: Command(gameObject)
			, m_MoveDir(0,0)
			, m_MoveDirRef(movedir)
			,useRef(true)
		{
			SetMoveComponent(gameObject);
		}

		void Execute(Seconds elapsedTime)
		{
			(elapsedTime);
			glm::vec2 moveVec = (useRef) ? *m_MoveDirRef : m_MoveDir;
			moveVec.y = -moveVec.y; // in this engine the Y is inverted
			m_MoveComponent->SetMoveDirection(moveVec);
		}
	private:
		MoveComponent* m_MoveComponent;
		
		bool useRef;
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
}