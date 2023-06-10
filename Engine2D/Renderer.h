#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "ImguiWrapper.h"

#include <vector>

namespace engine
{
	class Texture2D;
	class UpdateContext;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};
		bool m_showDemo{ false };

		int m_Width;
		int m_Height;

		//std::vector<float> m_FramerateHistory;
		//bool m_IsFramerateLimitChanged{ true };

		//struct FrametimeColorTypes
		//{
		//	ImColor poorPerformance{ 255, 20, 20 };
		//	ImColor moderatePerformance{ 255, 140, 0 };
		//	ImColor highPerformance{ 20, 255, 20 };
		//} m_FrametimeColorTypes;
	public:
		void Init(SDL_Window* window);
		void Render(UpdateContext& context);
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, SDL_RendererFlip flip) const;
		void RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, SDL_RendererFlip flip) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const int srcX, const int srcY, SDL_RendererFlip flip) const;
		void RenderTexture(const Texture2D& texture, const SDL_Rect& srcRect, const SDL_Rect& dstRect, SDL_RendererFlip flip) const;
		void RenderTextureBackground(const Texture2D& texture) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

