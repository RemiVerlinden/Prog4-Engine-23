#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include "Scene.h"
#include <assert.h>
#include "BaseComponent.h"

namespace dae
{
	class UpdateContext;
	class TransformComponent;
	class GameTime;
	// todo: this should become final.
	class GameObject final
	{
	public:
		void Update(const UpdateContext& context);
		void LateUpdate(const UpdateContext& context);
		void FixedUpdate(const UpdateContext& context);
		void Render() const;

		void SetPosition(float x, float y);

		void Destroy();
		inline bool IsMarkedForDestroy() { return m_MarkedForDestroy; }
		//===============================================
		template<typename ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args);


		template<typename ComponentType>
		ComponentType* GetComponent();

		template<typename ComponentType>
		bool HasComponent();

		template<typename ComponentType>
		void RemoveComponent();
		//===============================================

		void SetParent(GameObject* parent, bool keepWorldPosition);
		inline GameObject* GetParent() { return m_Parent; };
		inline const std::vector<GameObject*>& GetChildren() { return m_Children; };
		inline size_t GetChildCount() { return m_Children.size(); };
		GameObject* GetChildAt(unsigned int index);

		GameObject(Scene* scene);

		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	public:
		TransformComponent* m_Transform;

	private:
		void AddChild(GameObject* go);
		void RemoveChild(GameObject* go);
	private:

		std::vector<std::unique_ptr<BaseComponent>>		m_Components;
		Scene*											m_Scene;

		GameObject*										m_Parent;
		std::vector<GameObject*>						m_Children;
		bool											m_MarkedForDestroy;
	};






	template<typename ComponentType, typename... Args>
	[[maybe_unused]] ComponentType* GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		m_Components.push_back(std::make_unique<ComponentType>(std::forward<Args>(args)...));

		ComponentType* newComponent = GetComponent<ComponentType>();
		dynamic_cast<BaseComponent*>(newComponent)->RootInitialize(this);
		return newComponent;
	}

	template<typename ComponentType>
	ComponentType* GameObject::GetComponent()
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		if(!HasComponent<ComponentType>())
		{
			assert("GameObject::GetComponent -> no component of this type found in GameObject.");
		}

		for (const std::unique_ptr<BaseComponent>& component : m_Components)
		{
			ComponentType* castedComponent = dynamic_cast<ComponentType*>(component.get());
			if (castedComponent != nullptr)
			{
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
	void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<BaseComponent, ComponentType>(), "ComponentType has to be subclass from the BaseComponent");

		if(!HasComponent<ComponentType>())
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