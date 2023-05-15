#include "TextComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <iostream>
#include "ResourceManager.h"
#include "GameObject.h"


dae::TextComponent::TextComponent()
	: m_needsUpdate(false)
	, m_text("CALL SetText()")
	, m_FontColor{ 255,255,255,255 }
	, m_font(nullptr)
	, m_textTexture(nullptr)
{
}

void dae::TextComponent::SetColor(int r, int g, int b, int a)
{
	m_FontColor = { (Uint8)r,(Uint8)g,(Uint8)b,(Uint8)a };
}



void dae::TextComponent::Initialize()
{
	if (!m_font)
	{
		SetFont(ResourceManager::GetInstance().LoadFont("fonts/raju-bold.otf", 32));
	}
}

void dae::TextComponent::Update(const UpdateContext& context)
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
		const auto& pos = m_GameObject->m_Transform->GetWorldPosition() + m_Offset;
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	if (m_font)
		m_needsUpdate = true;

}

void dae::TextComponent::SetFont(std::shared_ptr<Font> font)
{
	m_font = font;
	m_needsUpdate = true;
}

void dae::TextComponent::SetPosition(const float x, const float y)
{
	m_Offset.x = x;
	m_Offset.y = y;
}

void dae::TextComponent::SetPosition(const glm::vec2& pos)
{
	SetPosition(pos.x, pos.y);
}

glm::vec3 dae::TextComponent::GetPosition()
{
	return m_GameObject->m_Transform->GetWorldPosition();
}

void dae::TextComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;

	auto componentClone = clone->AddComponent<TextComponent>(GetComponentTag());
	componentClone->SetText(m_text);
	componentClone->SetFont(m_font);
	componentClone->SetPosition(m_Offset);

	componentClone->SetColor(m_FontColor.r, m_FontColor.g, m_FontColor.b, m_FontColor.a);
}
