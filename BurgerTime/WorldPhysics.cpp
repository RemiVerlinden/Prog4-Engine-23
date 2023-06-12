#include "WorldPhysics.h"
#include "GameObject.h"
#include "WorldDataComponent.h"
#include "Locator.h"
#include "DebugRenderer.h"

using namespace engine::physics;

void engine::WorldPhysics::SolveWorldCollision(CirclePhysicsComponent* circlePhysicsComp)
{
	GameObject* pOwmer = circlePhysicsComp->GetGameObject();
	WorldDataComponent* pWorldData = static_cast<WorldDataComponent*>(pOwmer->GetScene()->GetWorldData());

	Circle& circle = circlePhysicsComp->GetCircleCollider();
	const Tile& currentTile = pWorldData->GetTileFromPosition(circle.pos);
	int depth = 1 + static_cast<int>(circle.radius / currentTile.collider.size.x); // minimum depth of 1

	std::vector<const Tile*> borderingTiles = pWorldData->GetBorderingTiles(currentTile, depth);
	m_DrawTiles = borderingTiles;

	IPhysicsSystem& physicsSystem = Locator::GetPhysicsSystem();


	for (const Tile* tile : borderingTiles)
	{
		if (tile->collisionType != CollisionType::Wall) continue;

		auto [isColliding, resolvedPosition] = physicsSystem.CheckCollisionCircleVsBox(circle, tile->collider);
		if (isColliding == true)
		{
			circle.pos = resolvedPosition;
			circlePhysicsComp->CollisionOccured();
		}
	}
}

void engine::WorldPhysics::Draw() const
{
	for (const Tile* tile : m_DrawTiles)
	{
		if (tile->collisionType != CollisionType::Wall) continue;

		glm::vec2 pos = tile->collider.pos;
		float size = tile->collider.size.x;

		SDL_Rect rect{ (int)pos.x,(int)pos.y, (int)size, (int)size };
		DebugRenderer::GetInstance().RenderRectangle(rect);
	}
}
