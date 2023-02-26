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

void dae::FpsComponent::Update()
{
	
	m_TextComponent->SetText(std::format("{} FPS", m_Time->GetFPS()));
	m_TextComponent->Update();
}

void dae::FpsComponent::Draw()
{
	m_TextComponent->Draw();
}

void dae::FpsComponent::LateUpdate()
{
	m_TextComponent->LateUpdate();
}

void dae::FpsComponent::FixedUpdate()
{
	m_TextComponent->FixedUpdate();
}

void dae::FpsComponent::SetPosition(const float x, const float y)
{
	m_TextComponent->SetPosition(x, y);
}

void dae::FpsComponent::SetColor(int r, int g, int b, int a)
{
	m_TextComponent->SetColor(r, g, b, a);
}

