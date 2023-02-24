#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace dae {

	class Texture2D;
	class Render2DComponent final : public BaseComponent
	{
	public:
		Render2DComponent();
		~Render2DComponent() = default;

		virtual void Initialize(GameTime* time);
		virtual void Update(float ts);
		virtual void Draw();
		virtual void LateUpdate(float ts);
		virtual void FixedUpdate(float ts);

		void SetTexture(const std::string& filename);
		void SetPosition(const float x, const float y);

		Render2DComponent(const Render2DComponent& other) = delete;
		Render2DComponent(Render2DComponent&& other) = delete;
		Render2DComponent& operator=(const Render2DComponent& other) = delete;
		Render2DComponent& operator=(Render2DComponent&& other) = delete;

	private:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_Texture;
	};
}