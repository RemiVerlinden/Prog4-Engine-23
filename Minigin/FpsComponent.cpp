#include "FpsComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <iostream>
#include "TextComponent.h"
#include <format>

#include "GameTime.h"

dae::FpsComponent::FpsComponent( std::shared_ptr<Font> font)
{
	m_TextComponent = std::make_shared<TextComponent>(font);
}

void dae::FpsComponent::Initialize([[maybe_unused]] GameTime* time)
{
	m_TextComponent->Initialize(time);
	m_Time = time;
}

void dae::FpsComponent::Update([[maybe_unused]] float ts)
{
	
	m_TextComponent->SetText(std::format("{} FPS", m_Time->GetFPS()));
	m_TextComponent->Update(ts);
}

void dae::FpsComponent::Draw()
{
	m_TextComponent->Draw();
}

void dae::FpsComponent::LateUpdate([[maybe_unused]] float ts)
{
	m_TextComponent->LateUpdate(ts);
}

void dae::FpsComponent::FixedUpdate([[maybe_unused]] float ts)
{
	m_TextComponent->FixedUpdate(ts);
}

void dae::FpsComponent::SetPosition(const float x, const float y)
{
	m_TextComponent->SetPosition(x, y);
}

void dae::FpsComponent::SetColor(int r, int g, int b, int a)
{
	m_TextComponent->SetColor(r, g, b, a);
}

