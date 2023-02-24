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
		void Update(float ts);
		void LateUpdate(float ts);
		void FixedUpdate(float ts);
		void Render() const;
		void Initialize(GameTime* time);

		//void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		//===============================================

		void SetTag(const std::string& tag) { m_Tag = tag; }

		template<typename T, typename... Args>
		std::weak_ptr<T> AddComponent(Args&&... args)
		{
			assert(!HasComponent<T>());
			std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>(args)...);
			m_Components.push_back(newComponent);
			return newComponent;
		}

		template<typename T>
		std::weak_ptr<T> GetComponent()
		{
			assert(HasComponent<T>());
			for (std::shared_ptr<BaseComponent> component : m_Components)
			{
				std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);
				if (castedComponent != nullptr)
				{
					return castedComponent;
				}
			}
		}
		template<typename T>
		bool HasComponent() 
		{
			for (std::shared_ptr<BaseComponent> component : m_Components)
			{
				if (std::dynamic_pointer_cast<T>(component) != nullptr)
				{
					return true;
				}
			}
			return false;
		}

		template<typename T>
		void RemoveComponent()
		{
			assert(HasComponent<T>());

			for (auto itr = m_Components.begin(); itr != m_Components.end(); ++itr)
			{
				std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(*itr);
				if (castedComponent != nullptr)
				{
					m_Components.erase(itr);
				}
			}
		};
		//===============================================

		GameObject() = default;
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
