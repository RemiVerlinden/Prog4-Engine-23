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
#include "GameTime.h"

dae::FpsComponent::FpsComponent( std::shared_ptr<Font> font)
{
	m_Font = font;
}

void dae::FpsComponent::Initialize([[maybe_unused]] GameTime* time)
{
	m_TextComponent = m_GameObject->AddComponent<TextComponent>(m_Font.lock());
	m_Time = time;
}

void dae::FpsComponent::Update()
{
	auto textComponent = m_TextComponent.lock();
	textComponent->SetText(std::format("{} FPS", m_Time->GetFPS()));
	textComponent->Update();
}

void dae::FpsComponent::Draw()
{
	m_TextComponent.lock()->Draw();
}

void dae::FpsComponent::LateUpdate()
{
	m_TextComponent.lock()->LateUpdate();
}

void dae::FpsComponent::FixedUpdate()
{
	m_TextComponent.lock()->FixedUpdate();
}

void dae::FpsComponent::SetPosition(const float x, const float y)
{
	m_TextComponent.lock()->SetPosition(x, y);
}

void dae::FpsComponent::SetColor(int r, int g, int b, int a)
{
	m_TextComponent.lock()->SetColor(r, g, b, a);
}

