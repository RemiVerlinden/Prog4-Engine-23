#pragma once
#include "IPhysicsSystem.hpp"


namespace dae
{
	class NullPhysicsSystem final : public IPhysicsSystem
	{
	public:
		NullPhysicsSystem() = default;
		virtual ~NullPhysicsSystem() = default;

		virtual bool CheckCollisionAABB(const Box&, const Box&) const { return false; };
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const Circle&, const Box&) const { return { false,{} }; };
		virtual std::pair<bool, glm::vec2> CheckCollisionCircleVsBox(const Box&, const Circle&) const { return { false,{} }; };
	};	
}