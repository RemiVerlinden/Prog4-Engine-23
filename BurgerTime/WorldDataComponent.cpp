#include "WorldDataComponent.h"
#include "Locator.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include <cassert>
#include "DebugRenderer.h"
#include "Structs.h"


#include <fstream>
#include <sstream>
#include <filesystem>
#include "RenderComponent.h"

using namespace engine;

// WorldDataComponent.cpp
std::unordered_map<engine::Scene*, bool> engine::WorldDataComponent::m_IsComponentInScene;


void engine::WorldDataComponent::Initialize()
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

	ParseWorldFromFile("stage1");

	m_TileRenderer = m_GameObject->AddComponent<RenderComponent>();
	if (std::filesystem::exists("../Data/Textures/stage1_tileset.tga"))
	{
		m_TileRenderer->SetTexture("../Data/Textures/stage1_tileset.tga");
	}
	else
	{
		std::cerr << "Texture not found!\n";
	}
	m_TileRenderer->SetDrawStyle(RenderComponent::DrawStyle::spritesheet);
	m_TileRenderer->SetResolution(static_cast<int>(m_WorldData.tileSize), static_cast<int>(m_WorldData.tileSize));
}

void engine::WorldDataComponent::Draw()
{
	// draw the tiles
	for (const Tile& tile : m_WorldData.tileList)
	{
		glm::vec2 pos = tile.collider.pos;

		glm::vec2 texturelocation{};
		texturelocation.x = static_cast<float>(tile.spriteType) * m_WorldData.defaultTileSize;

		m_TileRenderer->SetSourceRect(texturelocation.x, texturelocation.y, m_WorldData.defaultTileSize, m_WorldData.defaultTileSize);
		m_TileRenderer->SetDestinationRect(pos.x, pos.y, m_WorldData.defaultTileSize, m_WorldData.defaultTileSize);
		m_TileRenderer->Draw();
	}
}

bool engine::WorldDataComponent::IsWorldDataInitialized(Scene* pScene)
{
	return m_IsComponentInScene[pScene];
}

const engine::Tile& engine::WorldDataComponent::GetTileFromPosition(const glm::vec2& pos) const
{
	// we need to truncate the float by converting to size_t
	size_t tileX = static_cast<int>(pos.x / m_WorldData.defaultTileSize);
	size_t tileY = static_cast<int>(pos.y / m_WorldData.defaultTileSize);

	size_t tileIndex = tileY * m_WorldData.rows + tileX;
	return m_WorldData.tileList[tileIndex];
}

std::vector<const engine::Tile*> engine::WorldDataComponent::GetBorderingTiles(const Tile& tile, int depth)
{
	std::vector<const engine::Tile*> borderingTiles{};
	int width = m_WorldData.rows;
	int height = m_WorldData.columns;

	// Offsets to neighboring tiles (up, down, left, right)

	std::vector<std::pair<int, int>> offsets;
	for (int y = -depth; y <= depth; y++)
		for (int x = -depth; x <= depth; x++)
		{
			offsets.emplace_back(x, y);
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

using namespace engine;

#include <filesystem>

void engine::WorldDataComponent::ParseWorldFromFile(const std::string& stagename)
{
	std::filesystem::path filepath = "../Data/Map/";
	std::filesystem::path mapPath = filepath / stagename;

	// Parse worldinfo.txt
	std::filesystem::path worldInfoPath = filepath / "worldinfo.txt";
	if (!std::filesystem::exists(worldInfoPath))
	{
		std::cerr << "worldinfo.txt does not exist!\n";
		return;
	}

	std::ifstream worldFile(worldInfoPath);
	std::string line;

	getline(worldFile, line); // skip "rows"
	getline(worldFile, line);
	m_WorldData.rows = std::stoi(line);

	getline(worldFile, line); // skip "columns"
	getline(worldFile, line);
	m_WorldData.columns = std::stoi(line);

	m_WorldData.tileSize = engine::WindowSettings::scale * m_WorldData.defaultTileSize;

	m_WorldData.isInitialized = true;

	worldFile.close();

	// Reading Tile Data
	std::filesystem::path spriteFilePath = mapPath / (stagename + "_sprites.csv");
	std::filesystem::path collisionFilePath = mapPath / (stagename + "_collision.csv");
	if (!std::filesystem::exists(spriteFilePath) || !std::filesystem::exists(collisionFilePath))
	{
		std::cerr << "CSV files do not exist!\n";
		return;
	}

	float tileSize = m_WorldData.defaultTileSize;

	for (int y = 0; y < m_WorldData.columns; y++)
		for (int x = 0; x < m_WorldData.rows; x++)
		{
			Box collider{ {x * tileSize, y * tileSize }, { tileSize,tileSize } };
			m_WorldData.tileList.emplace_back(collider, x, y, SpriteType::uninitialized, CollisionType::uninitialized);
		}

	ParseSprites(spriteFilePath);
	ParseCollision(collisionFilePath);
}

void WorldDataComponent::ParseSprites(const std::filesystem::path& filePath)
{
	std::ifstream file(filePath);
	if (!file) {
		std::cerr << "Failed to open file: " << filePath << '\n';
		return;
	}

	std::string line;
	std::vector<SpriteType> spriteTypes;

	// Parse the data into a vector of CollisionType
	while (std::getline(file, line)) 
	{
		std::istringstream lineStream(line);
		std::string num;

		while (std::getline(lineStream, num, ',')) 
		{
			SpriteType type = static_cast<SpriteType>(std::stoi(num));
			spriteTypes.emplace_back(type);
		}
	}

	// Check if the collisionTypes vector is the same size as the tileList vector
	if (spriteTypes.size() != m_WorldData.tileList.size())
	{
		std::cerr << "Mismatch in number of tiles and collision types\n";
		return;
	}

	// Use a for loop to put the collision types in the tiles
	for (std::size_t i = 0; i < m_WorldData.tileList.size(); ++i)
	{
		m_WorldData.tileList[i].spriteType = spriteTypes[i];
	}
}

void WorldDataComponent::ParseCollision(const std::filesystem::path& filePath)
{
	std::ifstream file(filePath);
	if (!file) {
		std::cerr << "Failed to open file: " << filePath << '\n';
		return;
	}

	std::string line;
	std::vector<CollisionType> collisionTypes;

	// Parse the data into a vector of CollisionType
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);
		std::string num;

		while (std::getline(lineStream, num, ',')) {
			CollisionType type = (num == "1") ? CollisionType::Wall : CollisionType::None;
			collisionTypes.push_back(type);
		}
	}

	// Check if the collisionTypes vector is the same size as the tileList vector
	if (collisionTypes.size() != m_WorldData.tileList.size()) 
	{
		std::cerr << "Mismatch in number of tiles and collision types\n";
		return;
	}

	// Use a for loop to put the collision types in the tiles
	for (std::size_t i = 0; i < m_WorldData.tileList.size(); ++i) 
	{
		m_WorldData.tileList[i].collisionType = collisionTypes[i];
	}
}


