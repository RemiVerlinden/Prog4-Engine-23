#pragma once
#include <vector>
#include <variant>
#include "glm\glm.hpp"
#include "Structs.h"
namespace engine
{
	class GameObject;
	class ColliderComponent;




	class IPhysicsSystem	
	{
	public:
		IPhysicsSystem() = default;
		virtual ~IPhysicsSystem() = default;

		virtual bool CheckCollisionAABB(const physics::Box& a, const physics::Box& b) const = 0;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const physics::Circle& c, const physics::Box& b) const = 0;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const physics::Box& b, const physics::Circle& c) const = 0;
	};
}