#pragma once
#include "BaseComponent.h"
#include <vector>
#include <filesystem>
#include "../Minigin/IPhysicsSystem.hpp"
#include <unordered_map>
namespace dae
{
	enum class SpriteType
	{
		Empty,
		Wall
	};
	enum class CollisionType
	{
		None,
		Wall,
		Food
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
		static inline int tileCountHorizontal = 0;
		static inline int tileCountVertical = 0;
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
		
		void ParseWorldFromFile(const std::filesystem::path& tileCollisionPath);
	private:

		virtual void Clone(GameObject*) override {};

		WorldData m_WorldData;

		static std::unordered_map<Scene*, bool> m_IsComponentInScene;
	};
}