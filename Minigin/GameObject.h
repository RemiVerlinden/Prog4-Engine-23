#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject
	{
	public:
		virtual void Update();
		virtual void LateUpdate();
		virtual void FixedUpdate();
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		//===============================================

		BaseComponent& GetComponent(const std::string& name);
		void AddComponent(const std::string& name);
		void AddComponent(BaseComponent& component);

		void RemoveComponent(const std::string& name);
		void RemoveComponent(BaseComponent& component);
		//===============================================

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		// todo: mmm, every gameobject has a texture? Is that correct?
		std::shared_ptr<Texture2D> m_texture{};
	};
}
