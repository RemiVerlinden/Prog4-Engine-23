#pragma once
#include "BaseComponent.h"
#include "UpdateContext.h"
#include "EventListener.h"
#include "Prefab.h"
#include <unordered_map>
#include <memory>

namespace dae
{
	class SpawnerComponent final : public BaseComponent, public EventListener
	{
	public:
		SpawnerComponent() = default;
		void Initialize();

		void FixedUpdate ([[maybe_unused]] const UpdateContext& context) override {};
		void Update([[maybe_unused]] const UpdateContext& context) override {};
		void LateUpdate([[maybe_unused]] const UpdateContext& context) override {};

		void Draw() override {};
		void DrawUI([[maybe_unused]] UpdateContext& context) override {};

		void AddSpawnableObject(Prefab&& spawner);
		void SpawnGameObject(const std::string& objectName, Scene* pScene);
		GameObject* GetPrefabObject(const std::string& objectName);
		virtual void OnEvent(const Event& sent);

		SpawnerComponent(const SpawnerComponent& other) = delete;
		SpawnerComponent(SpawnerComponent&& other) = delete;
		SpawnerComponent& operator=(const SpawnerComponent& other) = delete;
		SpawnerComponent& operator=(SpawnerComponent&& other) = delete;
	private:
		void Clone(GameObject*) override {};

		inline static std::unordered_map<std::string, std::unique_ptr<Prefab>> m_SpawnableObjects;
	};
}

