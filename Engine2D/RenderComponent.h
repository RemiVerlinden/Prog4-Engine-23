#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"
#include "Structs.h"

namespace engine 
{

	class UpdateContext;
	class Texture2D;
	class RenderComponent final : public BaseComponent
	{
	public:


		RenderComponent();
		~RenderComponent() = default;

		virtual void Initialize() override;
		virtual void Draw();

		void SetTexture(const std::string& filename);
		void SetPosition(const float x, const float y);
		void SetResolution(const int width, const int height);
		void SetSourceRect(const float x, const float y, const float width, const float height );
		void SetDestinationRect(const float x, const float y, const float width, const float height );
		void SetDrawStyle(render::DrawStyle drawStyle) { m_DrawStyle = drawStyle; };
		void SetFlipTexture(render::FlipTexture flip) { m_FlipTexture = flip; };
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

	private:
		render::DrawStyle				m_DrawStyle;
		glm::ivec4						m_SrcRect;
		glm::ivec4						m_DstRect;
		glm::ivec2						m_TextureResolution;
		std::shared_ptr<Texture2D>		m_Texture;
		std::string						m_TextureFileName;
		render::FlipTexture				m_FlipTexture;
	private:
	};
}

