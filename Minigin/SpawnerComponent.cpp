#include "SpawnerComponent.h"
#include "GameObject.h"
#include "EventManager.h"
#include "Locator.h"

using namespace dae;

void dae::SpawnerComponent::Initialize()
{
	EventSubscribe(OnGameObjectSpawn);
}

void dae::SpawnerComponent::AddSpawnableObject(Prefab&& spawner)
{
	const std::string& name = spawner.GetPrefabObject()->GetComponent<TagComponent>()->m_Tag;

	if (m_SpawnableObjects.find(name) != m_SpawnableObjects.end())
	{
		ENGINE_ERROR("SpawnerComponent::SpawnGameObject: trying to add prefab with name {} to spawnerComponent but one with this exact name is already present.", name);
		assert(false);
	}
	m_SpawnableObjects.emplace(name, std::make_unique<Prefab>(std::move(spawner)));
}

void dae::SpawnerComponent::SpawnGameObject(const std::string& objectName, Scene* pScene)
{
	if (m_SpawnableObjects.find(objectName) != m_SpawnableObjects.end())
		m_SpawnableObjects[objectName]->Instantiate(pScene);
	else
		ENGINE_WARN("SpawnerComponent::SpawnGameObject: trying to SPAWN GameObject with name {}, but none was found.", objectName);

}

GameObject* dae::SpawnerComponent::GetPrefabObject(const std::string& objectName)
{
	if (m_SpawnableObjects.find(objectName) != m_SpawnableObjects.end())
		return m_SpawnableObjects[objectName]->GetPrefabObject();

	ENGINE_WARN("SpawnerComponent::GetPrefabObject: trying to GET prefab with name {}, but none was found.", objectName);
	return nullptr;
}

void dae::SpawnerComponent::OnEvent(const Event& sent)
{
	switch (sent.GetEventType())
	{
		case EventType::OnGameObjectSpawn:
		{
			const OnGameObjectSpawnEvent& event = reinterpret_cast<const OnGameObjectSpawnEvent&>(sent);

			Scene* pScene = SceneManager::GetInstance().GetActiveGameScene();
			SpawnGameObject(event.GetObjectName(), pScene);
		}
	}
}

