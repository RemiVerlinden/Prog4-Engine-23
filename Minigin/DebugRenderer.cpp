#include "DebugRenderer.h"

void dae::DebugRenderer::Init(SDL_Renderer* renderer)
{
    m_DebugRenderer = renderer;
}

void dae::DebugRenderer::RenderRectangle(const SDL_Rect& rect) const
{
    SDL_SetRenderDrawColor(m_DebugRenderer, m_DebugColor.r, m_DebugColor.g, m_DebugColor.b, m_DebugColor.a);
    SDL_RenderDrawRect(m_DebugRenderer, &rect);
}

void dae::DebugRenderer::RenderCircle(int x, int y, int radius) const
{
    SDL_SetRenderDrawColor(m_DebugRenderer, m_DebugColor.r, m_DebugColor.g, m_DebugColor.b, m_DebugColor.a);
    DrawCircle(x, y, radius);
}

void dae::DebugRenderer::DrawCircle(int32_t centreX, int32_t centreY, int32_t radius) const
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(m_DebugRenderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(m_DebugRenderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
