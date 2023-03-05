#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace dae {
	class Font;
	class Texture2D;
	class GameTime;
	class TextComponent;
class OrbitComponent final : public BaseComponent
{
	public:
		OrbitComponent() = default;
		~OrbitComponent() = default;

		virtual void Initialize(GameTime* time) override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;

		inline void SetRadius(float radius) { m_Radius = radius; }
		inline void SetSpeed(float speed) { m_Speed = speed; }

		OrbitComponent(const OrbitComponent& other) = delete;
		OrbitComponent(OrbitComponent&& other) = delete;
		OrbitComponent& operator=(const OrbitComponent& other) = delete;
		OrbitComponent& operator=(OrbitComponent&& other) = delete;

	private:
		TransformComponent* m_Transform;
		GameTime* m_Time;
		glm::vec3 m_CenterPos;

		float m_Speed = 10.f;
		float m_Radius = 10.f;
	};
};
