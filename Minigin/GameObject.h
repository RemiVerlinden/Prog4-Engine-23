#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include <unordered_set>

namespace dae
{
	class BaseComponent;
	class GameTime;
	// todo: this should become final.
	class GameObject final
	{
	public:
		void Update();
		void LateUpdate();
		void FixedUpdate();
		void Render() const;
		void Initialize(GameTime* time);

		void SetPosition(float x, float y);

		//===============================================

		void SetTag(const std::string& tag) { m_Tag = tag; }

		template<typename ComponentType, typename... Args>
		[[maybe_unused]] std::weak_ptr<ComponentType> AddComponent(Args&&... args)
		{
			assert(!HasComponent<ComponentType>());
			std::shared_ptr<ComponentType> newComponent = std::make_shared<ComponentType>(std::forward<Args>(args)...);
			m_Components.push_back(newComponent);
			return newComponent;
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
		//===============================================

		GameObject() = default;
		GameObject(const std::string& tag);

		~GameObject();
		
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		std::string m_Tag;
		// todo: mmm, every gameobject has a texture? Is that correct?
	};
}
