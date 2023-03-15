#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae
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
		bool m_showDemo{ true };

		int m_Width;
		int m_Height;
	public:
		void Init(SDL_Window* window);
		void Render(UpdateContext& context);
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTextureBackground(const Texture2D& texture) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

