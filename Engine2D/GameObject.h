#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include <assert.h>
#include <string>
#include <format>
#include "TagComponent.h"
#include "Scene.h"
#include "Locator.h"

namespace engine
{
	class UpdateContext;
	class TransformComponent;
	class GameTime;
	class BaseComponent;
	class GameObject final
	{
		friend void Scene::AddGameObject(std::shared_ptr<GameObject> object);

	public:
		// Update the components attached to the game object.
		void Update(const UpdateContext& context);

		// secondary update, after the defualt update.
		void LateUpdate(const UpdateContext& context);

		// Perform FixedUpdate on all components attached to the game object.
		void FixedUpdate(const UpdateContext& context);

		// Renders the components attached to this game object.
		void Render() const;
		
		// Renders the ImGui of the components attached to this game object.
		void RenderUI(UpdateContext& context) const;


		void OnSceneActivate();
		void OnSceneDeactivate();

		// Set the position of the game object.
		void SetPosition(float x, float y);

		void SetActive(bool active);

		bool IsActive() const;

		// Mark the game object for destruction.
		void Destroy();

		// Check if the game object is marked for destruction.
		inline bool IsMarkedForDestroy() { return m_MarkedForDestroy; }

		// Add a component to the game object.
		template<typename ComponentType>
		ComponentType* AddComponent(const std::string& componentTag = "");

		// Get a component from the game object.
		template<typename ComponentType>
		ComponentType* GetComponent(const std::string& componentTag = "");

		// Get a all components from the game object.
		template<typename ComponentType>
		std::vector<ComponentType*> GetAllComponents();
		
		// Check if the game object has a specific component.
		template<typename ComponentType>
		bool HasComponent();

		// Check if the game object has a specific component with a tag.
		template<typename ComponentType>
		bool HasComponentWithTag(const std::string& componentTag);

		// Get the amount of components of a specific type.
		template<typename ComponentType>
		int GetComponentCount();

		// Remove a component from the game object.
		template<typename ComponentType>
		void RemoveComponent();

		// Set the parent of the game object.
		void SetParent(GameObject* parent, bool keepWorldPosition);

		inline GameObject* GetParent() { return m_Parent; };
		inline const std::vector<GameObject*>& GetChildren() { return m_Children; };

		// Get the number of children of the game object.
		inline size_t GetChildCount() { return m_Children.size(); };

		// Get a child of the game object at specific index.
		GameObject* GetChildAt(unsigned int index);

		// Get the total count of game objects.
		static uint64_t GetGameObjectCount() { return m_GameObjectCount; }

		const std::string& GetTag() { return m_Tag; }

		Scene* GetScene() { return m_Scene; }

		GameObject(std::string tag);
		GameObject(Scene* scene, std::string tag);

		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	private:
		void AddChild(GameObject* go);
		void RemoveChild(GameObject* go);
	public:
		TransformComponent* m_Transform;

	private:
		std::string										m_Tag;
		Scene*											m_Scene;
		GameObject*										m_Parent;
		std::vector<GameObject*>						m_Children;
		std::vector<std::unique_ptr<BaseComponent>>		m_Components;
		bool											m_MarkedForDestroy;
		bool											m_IsActive;

		inline static uint64_t m_GameObjectCount = 0;
	};



	 
 

	//template<typename ComponentType, typename... Args>
	template<typename ComponentType>
	[[maybe_unused]] ComponentType* GameObject::AddComponent(const std::string& componentTag)
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		if (HasComponentWithTag<ComponentType>(componentTag))
		{
			const std::string& objectTag = GetComponent<TagComponent>()->m_Tag;

			ENGINE_ERROR("GameObject::AddComponent -> Component [{}] with tag:[{}] already present on GameObject With Tag[{}]", typeid(ComponentType).name(), componentTag, objectTag);
			assert(false);
		}

		m_Components.emplace_back(std::make_unique<ComponentType>());
		m_Components.back()->SetComponentTag(componentTag);

		ComponentType* newComponent = GetComponent<ComponentType>(componentTag);
		dynamic_cast<BaseComponent*>(newComponent)->RootInitialize(this);
		return newComponent;
	}

	template<typename ComponentType>
	ComponentType* GameObject::GetComponent(const std::string& componentTag)
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		if (!HasComponent<ComponentType>())
		{
			ENGINE_ERROR("GameObject::GetComponent -> no component of this type found in GameObject.");
			assert(false);
		}

		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			ComponentType* pCastedComponent = dynamic_cast<ComponentType*>(component.get());
			if (pCastedComponent != nullptr)
			{
				if (component->GetComponentTag() == componentTag)
					return pCastedComponent;
			}
		}
		return nullptr;
	}

	template<typename ComponentType>
	std::vector<ComponentType*> GameObject::GetAllComponents()
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		if (!HasComponent<ComponentType>())
		{
			ENGINE_ERROR("GameObject::GetComponent -> no component of this type found in GameObject.");
			assert(false);
		}

		std::vector<ComponentType*> components;
		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			ComponentType* pCastedComponent = dynamic_cast<ComponentType*>(component.get());
			if (pCastedComponent != nullptr)
			{
				components.emplace_back(pCastedComponent);
			}
		}
		return components;
	}

	template<typename ComponentType>
	bool GameObject::HasComponent()
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			if (dynamic_cast<ComponentType*>(component.get()) != nullptr)
			{
				return true;
			}
		}
		return false;
	}

	template<typename ComponentType>
	bool GameObject::HasComponentWithTag(const std::string& tag)
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			if (dynamic_cast<ComponentType*>(component.get()) != nullptr)
			{
				if (component->GetComponentTag() == tag)
					return true;
			}
		}
		return false;
	}

	template<typename ComponentType>
	int GameObject::GetComponentCount()
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		int componentCount = 0;
		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			if (dynamic_cast<ComponentType*>(component.get()) != nullptr)
			{
				++componentCount;
			}
		}
		return componentCount;
	}



	template<typename ComponentType>
	void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		if (!HasComponent<ComponentType>())
		{
			ENGINE_ERROR("GameObject::RemoveComponent -> no component of this type found in GameObject.");
			assert(false);
		}

		for (auto itr = m_Components.begin(); itr != m_Components.end(); ++itr)
		{
			ComponentType* castedComponent = dynamic_cast<ComponentType*>(itr->get());
			if (castedComponent != nullptr)
			{
				m_Components.erase(itr);
			}
		}
	};
}