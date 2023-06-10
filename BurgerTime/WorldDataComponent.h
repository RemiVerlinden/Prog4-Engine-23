#pragma once
#include "BaseComponent.h"
#include <vector>
#include <filesystem>

#include "../Engine2D/IPhysicsSystem.hpp"
#include <unordered_map>

namespace engine
{
	class Render2DComponent;
	enum class SpriteType
	{
		ladderTopLeftAlt,
		ladderTopRightAlt,
		floor1,
		floor2,
		floor3,
		ladderMiddleLeft,
		ladderMiddleRight,
		plateLeft,
		plateMiddle,
		plateRight,
		floor2Alt,
		floor3Alt,
		ladderTopleft,
		ladderTopRight,
		empty,
		uninitialized = -1
	};

	enum class CollisionType
	{
		None,
		Wall,
		Food,
		uninitialized
	};

	struct Tile
	{
		Box collider;
		int x;
		int y;
		SpriteType spriteType;
		CollisionType collisionType;
	};

	struct WorldData
	{
		bool isInitialized = false;
		static inline int rows = 0;
		static inline int columns = 0;
		const static inline float defaultTileSize = 8.f;
		static inline float tileSize = 0.f;
		std::vector<Tile> tileList;
	};

	class Scene;

	class WorldDataComponent final : public BaseComponent
	{
	public:
		WorldDataComponent() = default;
		~WorldDataComponent() = default;

		virtual void Initialize() override;

		virtual void Draw() override;

		WorldData& GetWorldData() { return m_WorldData; };
		static bool IsWorldDataInitialized(Scene* pScene);

		const Tile& GetTileFromPosition(const glm::vec2& pos) const;
		std::vector<const Tile*> GetBorderingTiles(const Tile& tile, int depth);
		
		void ParseWorldFromFile(const std::string& stagename);
	private:
		void ParseSprites(const std::filesystem::path& filePath);
		void ParseCollision(const std::filesystem::path& filePath);
		virtual void Clone(GameObject*) override {};

		WorldData m_WorldData;
		Render2DComponent* m_TileRenderer;
		static std::unordered_map<Scene*, bool> m_IsComponentInScene;
	};
}