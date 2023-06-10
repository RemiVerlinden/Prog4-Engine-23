#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace engine {

	class UpdateContext;
	class Texture2D;
	class Render2DComponent final : public BaseComponent
	{
	public:
		enum DrawStyle : int
		{
			normal, // render texture at position
			customResolution, // render texture at position with custom resolution
			spritesheet, // render only part of texture
			background // stretch texture to fit background
		};

		enum FlipTexture
		{
			SDL_FLIP_NONE = 0x00000000,     /**< Do not flip */
				SDL_FLIP_HORIZONTAL = 0x00000001,    /**< flip horizontally */
				SDL_FLIP_VERTICAL = 0x00000002     /**< flip vertically */
		};

		Render2DComponent();
		~Render2DComponent() = default;

		virtual void Initialize() override;
		virtual void Draw();

		void SetTexture(const std::string& filename);
		void SetPosition(const float x, const float y);
		void SetResolution(const int width, const int height);
		void SetSourceRect(const float x, const float y, const float width, const float height );
		void SetDestinationRect(const float x, const float y, const float width, const float height );
		void SetDrawStyle(DrawStyle drawStyle) { m_DrawStyle = drawStyle; };
		void SetFlipTexture(FlipTexture flip) { m_FlipTexture = flip; };
		Render2DComponent(const Render2DComponent& other) = delete;
		Render2DComponent(Render2DComponent&& other) = delete;
		Render2DComponent& operator=(const Render2DComponent& other) = delete;
		Render2DComponent& operator=(Render2DComponent&& other) = delete;

	private:
		DrawStyle						m_DrawStyle;
		glm::ivec4						m_SrcRect;
		glm::ivec4						m_DstRect;
		glm::ivec2						m_TextureResolution;
		std::shared_ptr<Texture2D>		m_Texture;
		std::string						m_TextureFileName;
		FlipTexture						m_FlipTexture;
	private:
	};
}

