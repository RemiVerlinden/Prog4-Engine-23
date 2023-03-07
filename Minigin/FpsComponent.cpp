#include "FpsComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <iostream>
#include "TextComponent.h"
#include <format>
#include "GameObject.h"
#include "UpdateContext.h"

dae::FpsComponent::FpsComponent( std::shared_ptr<Font> font)
{
	m_Font = font;
}

dae::FpsComponent::~FpsComponent()
{
}

void dae::FpsComponent::Initialize()
{
	m_TextComponent = m_GameObject->AddComponent<TextComponent>(m_Font);
	m_TextComponent->SetText(std::format("{} FPS", m_FramesPerSecond));
}

void dae::FpsComponent::Update([[maybe_unused]] const UpdateContext& context)
{
	m_Accumulator += context.GetDeltaTime();
	++m_FpsCount;
	if (m_Accumulator >= 1.0f)
	{
		m_FramesPerSecond = m_FpsCount;
		m_FpsCount = 0;
		m_Accumulator -= 1.0f;
		m_TextComponent->SetText(std::format("{} FPS", m_FramesPerSecond));
	}

}

void dae::FpsComponent::SetPosition(const float x, const float y)
{
	m_TextComponent->SetPosition(x, y);

}

void dae::FpsComponent::SetColor(int r, int g, int b, int a)
{
	m_TextComponent->SetColor(r, g, b, a);
}

