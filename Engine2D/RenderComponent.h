#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace engine {

	class UpdateContext;
	class Texture2D;
	class RenderComponent final : public BaseComponent
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

		RenderComponent();
		~RenderComponent() = default;

		virtual void Initialize() override;
		virtual void Draw();

		void SetTexture(const std::string& filename);
		void SetPosition(const float x, const float y);
		void SetResolution(const int width, const int height);
		void SetSourceRect(const float x, const float y, const float width, const float height );
		void SetDestinationRect(const float x, const float y, const float width, const float height );
		void SetDrawStyle(DrawStyle drawStyle) { m_DrawStyle = drawStyle; };
		void SetFlipTexture(FlipTexture flip) { m_FlipTexture = flip; };
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

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

