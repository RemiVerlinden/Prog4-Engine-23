#include "Scene.h"
#include "GameObject.h"
#include "IDComponent.h"
#include "TagComponent.h"
#include "SceneManager.h"
#include "Locator.h"
#include "NullWorldPhysics.h"

using namespace engine;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& tag) 
	: m_Tag(tag)
{
	// Initialize the timer in the scene
	m_pPhysicsWorld = std::make_unique<NullWorldPhysics>();
}

void engine::Scene::DeleteMarkedForDestroy()
{
	std::erase_if(m_objects,[](const std::shared_ptr<GameObject>& obj) 
		{
			return obj->IsMarkedForDestroy();
		});
}


Scene::~Scene() = default;

void Scene::AddGameObject(std::shared_ptr<GameObject> object)
{
	if (object->m_Scene != nullptr)
	{
		ENGINE_ERROR("Function Scene::AddGameObject -> trying to add GameObject that is already in a scene to another one: Undefined behavior");
		assert(false);
	}
	object->m_Scene = this;
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

void engine::Scene::LateUpdate(const UpdateContext& context)
{
	for (auto& object : m_objects)
	{
		object->LateUpdate(context);
	}

	DeleteMarkedForDestroy();

}

void engine::Scene::FixedUpdate(const UpdateContext& context)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(context);
	}
}

void engine::Scene::OnSceneActivate()
{
	for (auto& object : m_objects)
	{
		object->OnSceneActivate();
	}
}

void engine::Scene::OnSceneDeactivate()
{
	for (auto& object : m_objects)
	{
		object->OnSceneDeactivate();
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
	std::shared_ptr<GameObject> go{ std::make_shared<GameObject>(tag) };

	GameObject* ptr = go.get();
	AddGameObject(go);

	return ptr;
}

GameObject* engine::Scene::FindGameObjectByID(uint64_t ID)
{
	for (const auto& object : m_objects)
	{
		IDComponent* pIDComponent = object->GetComponent<IDComponent>();

		if (pIDComponent->GetID() == ID)
			return object.get();
	}

	return nullptr;
}

GameObject* engine::Scene::FindGameObjectByTag(std::string tag)
{
	for (const auto& object : m_objects)
	{
		TagComponent* pTagComponent = object->GetComponent<TagComponent>();

		if (pTagComponent->m_Tag == tag)
			return object.get();
	}

	return nullptr;
}
