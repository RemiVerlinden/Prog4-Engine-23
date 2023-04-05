#include "Scene.h"
#include "GameObject.h"
#include "IDComponent.h"
#include "TagComponent.h"
#include "SceneManager.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& tag) 
	: m_Tag(tag)
{
	// Initialize the timer in the scene
}

void dae::Scene::DeleteMarkedForDestroy()
{
	std::erase_if(m_objects,[](const std::shared_ptr<GameObject>& obj) 
		{
			return obj->IsMarkedForDestroy();
		});
}


Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(const UpdateContext& context)
{
	for(auto& object : m_objects)
	{
		object->Update(context);
	}
}

void dae::Scene::LateUpdate(const UpdateContext& context)
{
	for (auto& object : m_objects)
	{
		object->LateUpdate(context);
	}

	DeleteMarkedForDestroy();

}

void dae::Scene::FixedUpdate(const UpdateContext& context)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(context);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}
void Scene::RenderUI(UpdateContext& context) const
{
	for (const auto& object : m_objects)
	{
		object->RenderUI(context);
	}
}

GameObject* Scene::MakeGameObject(const std::string& tag )
{
	std::shared_ptr<GameObject> go{ std::make_shared<GameObject>(this, tag) };

	GameObject* ptr = go.get();

	Add(go);

	return ptr;
}

GameObject* dae::Scene::FindGameObjectByID(uint64_t ID)
{
	for (const auto& object : m_objects)
	{
		IDComponent* pIDComponent = object->GetComponent<IDComponent>();

		if (pIDComponent->GetID() == ID)
			return object.get();
	}

	return nullptr;
}

GameObject* dae::Scene::FindGameObjectByTag(std::string tag)
{
	for (const auto& object : m_objects)
	{
		TagComponent* pTagComponent = object->GetComponent<TagComponent>();

		if (pTagComponent->m_Tag == tag)
			return object.get();
	}

	return nullptr;
}
