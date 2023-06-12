#pragma once
#include "IPhysicsSystem.hpp"


namespace engine
{
	class NullPhysicsSystem final : public IPhysicsSystem
	{
	public:
		NullPhysicsSystem() = default;
		virtual ~NullPhysicsSystem() = default;

		virtual bool CheckCollisionAABB(const engine::physics::Box&, const engine::physics::Box&) const { return false; };
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const engine::physics::Circle&, const engine::physics::Box&) const { return { false,{} }; };
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const engine::physics::Box&, const engine::physics::Circle&) const { return { false,{} }; };
	};	
}