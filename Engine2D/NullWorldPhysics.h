#pragma once
#include "IWorldPhysics.h"

namespace engine
{
	class NullWorldPhysics : public IWorldPhysics
	{
	public:
		virtual ~NullWorldPhysics() = default;

		virtual void SolveWorldCollision(CirclePhysicsComponent*) override {};
	};
}

