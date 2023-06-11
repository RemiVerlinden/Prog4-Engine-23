#include "RenderComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include "glm\glm.hpp"
#include "GameObject.h"
#include "Structs.h"


engine::RenderComponent::RenderComponent()
	: m_Texture(nullptr)
	, m_DrawStyle(DrawStyle::normal)
	, m_SrcRect(0, 0, 1, 1)
	,m_FlipTexture(SDL_FLIP_NONE)
{

}

void engine::RenderComponent::Initialize()
{
	m_GameObject->m_Transform = m_GameObject->GetComponent<TransformComponent>();
}


void engine::RenderComponent::Draw()
{
	glm::vec2 pos = m_GameObject->m_Transform->GetWorldPosition() ;

	pos.x *= WindowSettings::scale;
	pos.y *= WindowSettings::scale;

	pos.x += m_DstRect.x;
	pos.y += m_DstRect.y;

	SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(m_FlipTexture);
	
	switch (m_DrawStyle)
	{
		case engine::RenderComponent::normal:
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, flip);
			break;
		case engine::RenderComponent::customResolution:
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, (float)m_DstRect.z, (float)m_DstRect.w, flip);
		}
		break;
		case engine::RenderComponent::spritesheet:
		{
			SDL_Rect srcRect, dstRect;
			srcRect.x = m_SrcRect.x;
			srcRect.y = m_SrcRect.y;
			srcRect.w = m_SrcRect.z;
			srcRect.h = m_SrcRect.w;

			dstRect.x = m_DstRect.x;
			dstRect.y = m_DstRect.y;
			dstRect.w = m_DstRect.z;
			dstRect.h = m_DstRect.w;

			Renderer::GetInstance().RenderTexture(*m_Texture, srcRect, dstRect, flip);
		}
		break;
		case engine::RenderComponent::background:
			Renderer::GetInstance().RenderTextureBackground(*m_Texture);
			break;
		default:
			assert(false);
			break;
	}
}




void engine::RenderComponent::SetTexture(const std::string& filename)
{

	m_TextureFileName = filename;
	m_Texture = ResourceManager::GetInstance().LoadTexture(m_TextureFileName);

	if (m_TextureResolution.x == 0 && m_TextureResolution.y == 0 && m_Texture)
	{
		m_TextureResolution = m_Texture->GetSize();
	}

}

void engine::RenderComponent::SetPosition(const float x, const float y)
{
	float scale = WindowSettings::scale;
	m_DstRect.x = static_cast<int>(x * scale);
	m_DstRect.y = static_cast<int>(y * scale);
}


void engine::RenderComponent::SetResolution(const int width, const int height)
{
	float scale = WindowSettings::scale;
	m_DstRect.z = static_cast<int>(width * scale);
	m_DstRect.w = static_cast<int>(height * scale);
}


void engine::RenderComponent::SetSourceRect(const float x, const float y, const float width, const float height)
{
	m_SrcRect.x = static_cast<int>(x);
	m_SrcRect.y = static_cast<int>(y);
	m_SrcRect.z = static_cast<int>(width);
	m_SrcRect.w = static_cast<int>(height);
}


void engine::RenderComponent::SetDestinationRect(const float x, const float y, const float width, const float height)
{
	float scale = WindowSettings::scale;
	m_DstRect.x = static_cast<int>(x * scale);
	m_DstRect.y = static_cast<int>(y * scale);
	m_DstRect.z = static_cast<int>(width * scale);
	m_DstRect.w = static_cast<int>(height * scale);
}


