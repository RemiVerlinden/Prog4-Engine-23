#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "UpdateContext.h"
#include <iostream>
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
		MoveCommand(GameObject* gameObject, glm::vec3 movedir)
			: Command(gameObject)
			, m_MoveDir(movedir)
			, m_Transform(GetActor()->m_Transform)
		{
		}

		MoveCommand(GameObject* gameObject, glm::vec2 movedir)
			: MoveCommand(gameObject, { movedir.x,movedir.y,0 })
		{
		}

		void Execute(Seconds elapsedTime)
		{
			glm::vec3 moveVec = m_MoveDir * speed * elapsedTime.ToFloat();
			*m_Transform += moveVec;
		}
	private:
		TransformComponent* m_Transform;
		glm::vec3 m_MoveDir;
		float speed = 100.f;
	};
}