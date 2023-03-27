#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "ImguiWrapper.h"

#include <vector>

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

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTextureBackground(const Texture2D& texture) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

