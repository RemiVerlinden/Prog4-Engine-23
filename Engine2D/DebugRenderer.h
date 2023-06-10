#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace engine
{
    class DebugRenderer final : public Singleton<DebugRenderer>
    {
    private:
        SDL_Renderer* m_DebugRenderer{};
        SDL_Color m_DebugColor{ 255, 0, 0, 255 }; // Default to red for visibility

    public:
        void Init(SDL_Renderer* renderer);
        void RenderRectangle(const SDL_Rect& rect) const;
        void RenderCircle(int x, int y, int radius) const;
        void SetDebugColor(const SDL_Color& color) { m_DebugColor = color; }

    private:
        void DrawCircle(int32_t centreX, int32_t centreY, int32_t radius) const;
    };
}
