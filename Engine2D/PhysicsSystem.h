#pragma once
#include "IPhysicsSystem.hpp"

namespace engine
{
	class PhysicsSystem final : public IPhysicsSystem
	{
	public:
		PhysicsSystem() = default;
		virtual ~PhysicsSystem() = default;

		virtual bool CheckCollisionAABB(const physics::Box& a, const physics::Box& b) const;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const physics::Circle& c, const physics::Box& b) const;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const physics::Box& b, const physics::Circle& c) const;
	};
}
