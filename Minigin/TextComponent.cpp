#include "TextComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <iostream>
#include "GameTime.h"

dae::TextComponent::TextComponent(std::shared_ptr<Font> font) 
	: TextComponent("",font) 
{}

dae::TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font)
	: m_needsUpdate(true)
	, m_text(text)
	, m_FontColor{255,255,255,255}
	, m_font(std::move(font))
	, m_textTexture(nullptr)
{}

void dae::TextComponent::SetColor(int r, int g, int b, int a)
{
	m_FontColor = { (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)a };
}


void dae::TextComponent::Initialize([[maybe_unused]] GameTime* time)
{

}

void dae::TextComponent::Update([[maybe_unused]] float ts)
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_FontColor);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Draw()
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::LateUpdate([[maybe_unused]] float ts)
{

}

void dae::TextComponent::FixedUpdate([[maybe_unused]] float ts)
{

}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
