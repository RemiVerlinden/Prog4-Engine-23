#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace dae {
	class UpdateContext;
class OrbitComponent final : public BaseComponent
{
	public:
		OrbitComponent() = default;
		~OrbitComponent() = default;

		virtual void Initialize() override;
		virtual void Update(const UpdateContext& context) override;

		inline void SetRadius(float radius) { m_Radius = radius; }
		inline void SetSpeed(float speed) { m_Speed = speed; }

		OrbitComponent(const OrbitComponent& other) = delete;
		OrbitComponent(OrbitComponent&& other) = delete;
		OrbitComponent& operator=(const OrbitComponent& other) = delete;
		OrbitComponent& operator=(OrbitComponent&& other) = delete;

	private:
		void Clone (GameObject* clone) override;
		glm::vec3					m_CenterPos;

		float						m_Speed = 10.f;
		float						m_Radius = 10.f;
	};
};
