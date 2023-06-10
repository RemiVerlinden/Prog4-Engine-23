#pragma once
#include "IPhysicsSystem.hpp"

namespace engine
{
	class PhysicsSystem final : public IPhysicsSystem
	{
	public:
		PhysicsSystem() = default;
		virtual ~PhysicsSystem() = default;

		virtual bool CheckCollisionAABB(const Box& a, const Box& b) const;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const Circle& c, const Box& b) const;
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const Box& b, const Circle& c) const;
	};
}
