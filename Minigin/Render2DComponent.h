#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace dae {

	class Texture2D;
	class Render2DComponent final : public BaseComponent
	{
	public:
		enum DrawStyle : int
		{
			position, // render texture at position
			positionScale, // render texture at position with custom scale
			background // stretch texture to fit background
		};

		Render2DComponent();
		~Render2DComponent() = default;

		virtual void Initialize(GameTime* time);
		virtual void Update();
		virtual void Draw();
		virtual void LateUpdate();
		virtual void FixedUpdate();

		void SetTexture(const std::string& filename);
		void SetPosition(const float x, const float y);
		void SetResolution(const int width, const int height);
		void SetDrawStyle(DrawStyle drawStyle) { m_DrawStyle = drawStyle; };

		Render2DComponent(const Render2DComponent& other) = delete;
		Render2DComponent(Render2DComponent&& other) = delete;
		Render2DComponent& operator=(const Render2DComponent& other) = delete;
		Render2DComponent& operator=(Render2DComponent&& other) = delete;

	private:
		DrawStyle m_DrawStyle;
		std::weak_ptr<TransformComponent> m_transform{};
		glm::vec4 m_SrcRect;
		glm::ivec2 m_TextureResolution;
		std::shared_ptr<Texture2D> m_Texture;

	};
}