#pragma once
#include "IWorldPhysics.h"
#include <vector>

namespace engine
{
	struct Tile;
	class WorldPhysics final : public IWorldPhysics
	{
	public:
		virtual ~WorldPhysics() = default;
		virtual void SolveWorldCollision(CirclePhysicsComponent*) override;

		void Draw() const;
		std::vector<const Tile*> m_DrawTiles;
	};
}