#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class GameTime;
	class TransformComponent final : public BaseComponent
	{
	public:


		TransformComponent() = default;
		~TransformComponent() = default;

		virtual void Initialize(GameTime * time) override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);

		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z);

		TransformComponent(const TransformComponent & other) = delete;
		TransformComponent(TransformComponent && other) = delete;
		TransformComponent& operator=(const TransformComponent & other) = delete;
		TransformComponent& operator=(TransformComponent && other) = delete;
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
	};
}
