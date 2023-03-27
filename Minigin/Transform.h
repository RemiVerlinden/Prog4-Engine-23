#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class UpdateContext;
	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent() = default;
		~TransformComponent() = default;

		void Initialize() override {};


		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& pos);
		void TranslatePosition(float x, float y, float z);
		void TranslatePosition(const glm::vec3& pos);
		const glm::vec3& GetLocalPosition() { return m_LocalPosition; }
		const glm::vec3& GetWorldPosition();

		inline void SetPositionDirty();
		inline void UpdateWorldPosition();

		TransformComponent(const TransformComponent & other) = delete;
		TransformComponent(TransformComponent && other) = delete;
		TransformComponent& operator=(const TransformComponent & other) = delete;
		TransformComponent& operator=(TransformComponent && other) = delete;

		TransformComponent& operator+=(const glm::vec3& offset);

	private:
		glm::vec3				m_LocalPosition;
		glm::vec3				m_WorldPosition;
		bool					m_IsDirty = true;
	};
}
