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

void dae::FpsComponent::Initialize()
{
	m_TextComponent = m_GameObject->AddComponent<TextComponent>(m_Font.lock());
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
	}

	auto textComponent = m_TextComponent.lock();
	textComponent->SetText(std::format("{} FPS", m_FramesPerSecond));
}

void dae::FpsComponent::Draw()
{
}

void dae::FpsComponent::LateUpdate([[maybe_unused]]const UpdateContext& context)
{
}

void dae::FpsComponent::FixedUpdate ([[maybe_unused]] const UpdateContext& context)
{
}

void dae::FpsComponent::SetPosition(const float x, const float y)
{
	m_TextComponent.lock()->SetPosition(x, y);

}

void dae::FpsComponent::SetColor(int r, int g, int b, int a)
{
	m_TextComponent.lock()->SetColor(r, g, b, a);
}

