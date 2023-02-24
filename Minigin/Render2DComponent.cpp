#include "Render2DComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameTime.h"

dae::Render2DComponent::Render2DComponent()
	: m_Texture(nullptr)
{

}

void dae::Render2DComponent::Initialize([[maybe_unused]]  GameTime* time)
{

}

void dae::Render2DComponent::Update([[maybe_unused]] float ts)
{

}

void dae::Render2DComponent::Draw()
{
	const auto& pos = m_transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::Render2DComponent::LateUpdate([[maybe_unused]] float ts)
{

}

void dae::Render2DComponent::FixedUpdate([[maybe_unused]] float ts)
{

}

void dae::Render2DComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::Render2DComponent::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
