#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include "Scene.h"
#include <assert.h>
#include "BaseComponent.h"

namespace dae
{
	class TransformComponent;
	class GameTime;
	// todo: this should become final.
	class GameObject final
	{
	public:
		void Update();
		void LateUpdate();
		void FixedUpdate();
		void Render() const;

		void SetPosition(float x, float y);

		//===============================================

		template<typename ComponentType, typename... Args>
		[[maybe_unused]] std::weak_ptr<ComponentType> AddComponent(Args&&... args)
		{
			std::shared_ptr<BaseComponent> newComponent = std::make_shared<ComponentType>(std::forward<Args>(args)...);
			m_Components.push_back(newComponent);
			newComponent->RootInitialize(this, m_Scene->GetGameTime());
			return std::dynamic_pointer_cast<ComponentType>(newComponent);
		}

		template<typename ComponentType>
		std::weak_ptr<ComponentType> GetComponent()
		{
			assert(HasComponent<ComponentType>());
			for (std::shared_ptr<BaseComponent> component : m_Components)
			{
				std::shared_ptr<ComponentType> castedComponent = std::dynamic_pointer_cast<ComponentType>(component);
				if (castedComponent != nullptr)
				{
					return castedComponent;
				}
			}
			return std::shared_ptr<ComponentType>(nullptr);
		}
		template<typename ComponentType>
		bool HasComponent()
		{
			for (std::shared_ptr<BaseComponent> component : m_Components)
			{
				if (std::dynamic_pointer_cast<ComponentType>(component) != nullptr)
				{
					return true;
				}
			}
			return false;
		}

		template<typename ComponentType>
		void RemoveComponent()
		{
			assert(HasComponent<ComponentType>());

			for (auto itr = m_Components.begin(); itr != m_Components.end(); ++itr)
			{
				std::shared_ptr<ComponentType> castedComponent = std::dynamic_pointer_cast<ComponentType>(*itr);
				if (castedComponent != nullptr)
				{
					m_Components.erase(itr);
				}
			}
		};

		void SetParent(GameObject* parent, bool keepWorldPosition);
		inline GameObject* GetParent() { return m_Parent; };
		inline size_t GetChildCount() { return m_Children.size(); };
		GameObject* GetChildAt(unsigned int index);
		//===============================================

		GameObject( Scene* scene );

		~GameObject();
		
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void AddChild(GameObject* go);
		void RemoveChild(GameObject* go);
	private:

		std::weak_ptr<TransformComponent> m_transform;
		std::vector<std::shared_ptr<BaseComponent>> m_Components;

		Scene* m_Scene;

		GameObject* m_Parent;
		std::vector<GameObject*> m_Children;
		// todo: mmm, every gameobject has a texture? Is that correct?
	};
}



