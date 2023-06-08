#pragma once
#include <vector>
#include <variant>
#include "glm\glm.hpp"

namespace dae
{
	class GameObject;
	class ColliderComponent;


	struct Box
	{
		glm::vec2 pos;
		glm::vec2 size;
	};

	struct Circle
	{
		glm::vec2 pos;
		float radius;
	};

	using Shape = std::variant<Box, Circle>;

	class IPhysicsSystem	
	{
	public:
		IPhysicsSystem() = default;
		virtual ~IPhysicsSystem() = default;

		virtual bool CheckCollisionAABB(const Box& a, const Box& b) const = 0;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const Circle& c, const Box& b) const = 0;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const Box& b, const Circle& c) const = 0;
	};
}