#include "Render2DComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "glm\glm.hpp"

dae::Render2DComponent::Render2DComponent()
	: m_Texture(nullptr)
	, m_DrawStyle(DrawStyle::position)
{

}

void dae::Render2DComponent::Initialize([[maybe_unused]] GameTime* time)
{
}

void dae::Render2DComponent::Update()
{

}

void dae::Render2DComponent::Draw()
{
	[[maybe_unused]] const auto& pos = m_transform.GetPosition();

	switch (m_DrawStyle)
	{
	case dae::Render2DComponent::position:
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
		break;
	case dae::Render2DComponent::positionScale:
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, (float)m_TextureResolution.x, (float)m_TextureResolution.y);
	}
		break;
	case dae::Render2DComponent::background:
		Renderer::GetInstance().RenderTextureBackground(*m_Texture);
		break;
	default:
		assert(false);
		break;
	}
}

void dae::Render2DComponent::LateUpdate()
{

}

void dae::Render2DComponent::FixedUpdate()
{

}

void dae::Render2DComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);

	if (m_TextureResolution.x == 0 && m_TextureResolution.y == 0 && m_Texture)
	{
		m_TextureResolution = m_Texture->GetSize();
	}

}

void dae::Render2DComponent::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::Render2DComponent::SetResolution(const int width, const int height)
{
	m_TextureResolution.x = width;
	m_TextureResolution.y = height;
}

