#include "Prefab.h"
#include "GameObject.h"
#include "Components.h"
#include "Scene.h"

using namespace dae;

Prefab::Prefab(const std::string& name) 
	:m_PrefabObject(std::make_unique<GameObject>(name))
{
}

GameObject* dae::Prefab::Instantiate(Scene* pScene)
{
	std::shared_ptr<GameObject> go = m_PrefabObject->Clone();


	pScene->AddGameObject(go);
	return go.get();
}

GameObject* dae::Prefab::GetPrefabObject()
{
	return m_PrefabObject.get();
}
