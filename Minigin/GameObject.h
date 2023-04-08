#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include <assert.h>
#include <string>
#include <format>
#include "TagComponent.h"

namespace dae
{
	class UpdateContext;
	class TransformComponent;
	class GameTime;
	class Scene;
	class BaseComponent;
	class GameObject final
	{
	public:
		void Update(const UpdateContext& context);
		void LateUpdate(const UpdateContext& context);
		void FixedUpdate(const UpdateContext& context);
		void Render() const;
		void RenderUI(UpdateContext& context) const;

		void SetPosition(float x, float y);

		void Destroy();
		inline bool IsMarkedForDestroy() { return m_MarkedForDestroy; }

		//===============================================
		template<typename ComponentType>
		//template<typename ComponentType, typename... Args>
		ComponentType* AddComponent(const std::string& componentTag = "");


		template<typename ComponentType>
		ComponentType* GetComponent(const std::string& componentTag = "");

		template<typename ComponentType>
		bool HasComponent();

		template<typename ComponentType>
		bool HasComponentWithTag(const std::string& componentTag);

		template<typename ComponentType>
		int GetComponentCount();


		template<typename ComponentType>
		void RemoveComponent();
		//===============================================

		void SetParent(GameObject* parent, bool keepWorldPosition);
		inline GameObject* GetParent() { return m_Parent; };
		inline const std::vector<GameObject*>& GetChildren() { return m_Children; };
		inline size_t GetChildCount() { return m_Children.size(); };
		GameObject* GetChildAt(unsigned int index);

		static uint64_t GetGameObjectCount() { return m_GameObjectCount; }
		const std::string& GetTag() { return m_Tag; }

		GameObject(Scene* scene);
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
		Scene* m_Scene;
		GameObject* m_Parent;
		std::vector<GameObject*>						m_Children;
		std::vector<std::unique_ptr<BaseComponent>>		m_Components;
		bool											m_MarkedForDestroy;

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
			assert(false && std::format("GameObject::AddComponent -> Component with tag:[{}] already present on GameObject With Tag[{}]", componentTag, objectTag).c_str());
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
			assert(false && "GameObject::GetComponent -> no component of this type found in GameObject.");
		}

		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			ComponentType* castedComponent = dynamic_cast<ComponentType*>(component.get());
			if (castedComponent != nullptr)
			{
				if (component->GetComponentTag() == componentTag)
					return castedComponent;
			}
		}
		return nullptr;
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
			assert("GameObject::RemoveComponent -> no component of this type found in GameObject.");
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