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

		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& pos);
		inline const glm::vec3& GetLocalPosition() { return m_LocalPosition; }
		const glm::vec3& GetWorldPosition();

		void SetPositionDirty();

		TransformComponent(const TransformComponent & other) = delete;
		TransformComponent(TransformComponent && other) = delete;
		TransformComponent& operator=(const TransformComponent & other) = delete;
		TransformComponent& operator=(TransformComponent && other) = delete;
	private:
		glm::vec3				m_LocalPosition;
		glm::vec3				m_WorldPosition;
		bool					m_DirtyFlag = true;
	};
}
