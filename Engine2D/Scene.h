#pragma once
#include "SceneManager.h"
#include "IWorldPhysics.h"

namespace engine
{
	class UpdateContext;
	class GameObject;
	class Scene final
	{
		friend engine::Scene* SceneManager::AddGameScene(const std::string& name);

		friend class GameObject;
	public:
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update(const UpdateContext& context);
		void Render() const;
		void RenderUI(UpdateContext& context) const;

		void LateUpdate(const UpdateContext& context);
		void FixedUpdate(const UpdateContext& context);

		void OnSceneActivate();
		void OnSceneDeactivate();

		//========================================================
		std::string GetTag() { return m_Tag; };
		GameObject* MakeGameObject(const std::string& tag = "No Name Defined");
		GameObject* FindGameObjectByID( uint64_t ID);
		GameObject* FindGameObjectByTag( std::string tag);
		//========================================================

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void AddGameObject(std::shared_ptr<GameObject> object);

		// I have PhysicsComponents, these will use the physics world to check for collisions
		IWorldPhysics& GetPhysicsWorld() { return *m_pPhysicsWorld; }
		void SetPhysicsWorld(std::unique_ptr<IWorldPhysics> physicsWorld) { m_pPhysicsWorld = std::move(physicsWorld); }

		void SetWorldData(void* data) { m_pWorldData = data; }
		void* GetWorldData() { return m_pWorldData; }
	private: 
		explicit Scene(const std::string& name);
		
		void DeleteMarkedForDestroy();

		std::string m_Tag;
		std::vector <std::shared_ptr<GameObject>> m_objects{};

		// I have PhysicsComponents, these will use the physics world to check for collisions
		std::unique_ptr<IWorldPhysics> m_pPhysicsWorld = nullptr;
		// you can do anything with this but I use it to store the some world data class that I make in the application
		void* m_pWorldData = nullptr;

		static unsigned int m_idCounter; 

		//========================================================

		//========================================================
	};

}


