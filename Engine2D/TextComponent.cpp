#include "TextComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <iostream>
#include "ResourceManager.h"
#include "GameObject.h"
#include "Structs.h"

engine::TextComponent::TextComponent()
	: m_needsUpdate(false)
	, m_text("CALL SetText()")
	, m_FontColor{ 255,255,255,255 }
	, m_font(nullptr)
	, m_textTexture(nullptr)
{
}

void engine::TextComponent::SetColor(int r, int g, int b, int a)
{
	m_FontColor = { (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)a };
	m_needsUpdate = true;
}



void engine::TextComponent::Initialize()
{
	if (!m_font)
	{
		SetFont(ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 32));
	}
}

void engine::TextComponent::Update(const UpdateContext& /*context*/)
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

void engine::TextComponent::Draw()
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = m_GameObject->m_Transform->GetWorldPosition() + m_Offset;
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y, SDL_FLIP_NONE);
	}
}

// This implementation uses the "dirty flag" pattern
void engine::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	if (m_font)
		m_needsUpdate = true;

}

void engine::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_font = font;
	m_needsUpdate = true;
}

void engine::TextComponent::SetPosition(const float x, const float y)
{
	float scale = WindowSettings::scale;
	m_Offset.x = x * scale;
	m_Offset.y = y * scale;
}

void engine::TextComponent::SetPosition(const glm::vec2& pos)
{
	SetPosition(pos.x, pos.y);
}

glm::vec3 engine::TextComponent::GetPosition()
{
	return m_GameObject->m_Transform->GetWorldPosition();
}

