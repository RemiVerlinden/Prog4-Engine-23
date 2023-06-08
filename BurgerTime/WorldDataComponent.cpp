#include "WorldDataComponent.h"
#include "Locator.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include <cassert>
#include "DebugRenderer.h"

// WorldDataComponent.cpp
std::unordered_map<dae::Scene*, bool> dae::WorldDataComponent::m_IsComponentInScene;


void dae::WorldDataComponent::Initialize()
{
	Scene* pScene = m_GameObject->GetScene();
	// check if the map already contains a bool with the scene* as key
	// if it doesn't, the map will add a new entry with the scene* as key and false as value
	bool& componentAlreadyInScene = m_IsComponentInScene[pScene];

	if (!componentAlreadyInScene)
	{
		pScene->SetWorldData(this);
		componentAlreadyInScene = true;
	}

	ParseWorldFromFile("empty");
}

void dae::WorldDataComponent::Draw()
{
	//// draw the tiles
	//for (const Tile& tile : m_WorldData.tileList)
	//{
	//	if(tile.collisionType != CollisionType::Wall) continue;
	//	glm::vec2 pos = tile.collider.pos;
	//	glm::vec2 size = tile.collider.size;
	//	SDL_Rect rect{(int)pos.x,(int)pos.y, (int)size.x, (int)size.y};
	//	DebugRenderer::GetInstance().RenderRectangle(rect);
	//}
}

bool dae::WorldDataComponent::IsWorldDataInitialized(Scene* pScene)
{
	return m_IsComponentInScene[pScene];
}

const dae::Tile& dae::WorldDataComponent::GetTileFromPosition(const glm::vec2& pos) const
{
	// we need to truncate the float by converting to size_t
	size_t tileX = static_cast<int>(pos.x / m_WorldData.tileSize);
	size_t tileY = static_cast<int>(pos.y / m_WorldData.tileSize);

	size_t tileIndex = tileY * m_WorldData.tileCountHorizontal + tileX;
	return m_WorldData.tileList[tileIndex];
}

std::vector<const dae::Tile*> dae::WorldDataComponent::GetBorderingTiles(const Tile& tile, int depth)
{
	std::vector<const dae::Tile*> borderingTiles{};
	int width = m_WorldData.tileCountHorizontal;
	int height = m_WorldData.tileCountVertical;

	// Offsets to neighboring tiles (up, down, left, right)

	std::vector<std::pair<int, int>> offsets;
	for (int x = -depth; x <= depth; x++)
	{
		for (int y = -depth; y <= depth; y++)
		{
			offsets.emplace_back(x, y);
		}
	}

	for (auto& [x, y] : offsets) {
		int newX = tile.x + x;
		int newY = tile.y + y;

		// Check if newX and newY are within the grid
		if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
			// Convert 2D indices to a 1D index
			int index = newY * width + newX;
			borderingTiles.push_back(&m_WorldData.tileList[index]);
		}
	}

	return borderingTiles;
}


void dae::WorldDataComponent::ParseWorldFromFile(const std::filesystem::path& /*tileCollisionPath*/)
{

	std::string worldMap =
		"########"
		"#......#"
		"#......#"
		"#....###"
		"#......#"
		"########";
	int tileCountX = 8;
	int tileCountY = 6;
	float tileSize = 80.f;
	m_WorldData.tileCountHorizontal = tileCountX;
	m_WorldData.tileCountVertical = tileCountY;
	m_WorldData.tileSize = tileSize;

	for (int y = 0; y < tileCountY; y++)
		for (int x = 0; x < tileCountX; x++)
		{
			char tileType = worldMap[y * tileCountX + x];
			Box collider{ {x * tileSize, y * tileSize }, { tileSize,tileSize } };
			SpriteType spriteType;
			CollisionType collisionType;
			if (tileType == '#')
			{
				spriteType = SpriteType::Wall;
				collisionType = CollisionType::Wall;
			}
			else
			{
				spriteType = SpriteType::Empty;
				collisionType = CollisionType::None;
			}
			m_WorldData.tileList.emplace_back(collider, x, y, spriteType, collisionType);
		}

	m_WorldData.isInitialized = true;
}
