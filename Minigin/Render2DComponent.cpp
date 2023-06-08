#include "Render2DComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include "glm\glm.hpp"
#include "GameObject.h"

dae::Render2DComponent::Render2DComponent()
	: m_Texture(nullptr)
	, m_DrawStyle(DrawStyle::position)
	, m_SrcRect(0, 0, 1, 1)
{

}

void dae::Render2DComponent::Initialize()
{
	m_GameObject->m_Transform = m_GameObject->GetComponent<TransformComponent>();
}


void dae::Render2DComponent::Draw()
{
	glm::vec2 pos = m_GameObject->m_Transform->GetWorldPosition();

	pos.x += m_SrcRect.x;
	pos.y += m_SrcRect.y;

	switch (m_DrawStyle)
	{
		case dae::Render2DComponent::position:
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
			break;
		case dae::Render2DComponent::positionScale:
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, (float)m_SrcRect.z, (float)m_SrcRect.w);
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

void dae::Render2DComponent::SetTexture(const std::string& filename)
{
	m_TextureFileName = filename;
	m_Texture = ResourceManager::GetInstance().LoadTexture(m_TextureFileName);

	if (m_TextureResolution.x == 0 && m_TextureResolution.y == 0 && m_Texture)
	{
		m_TextureResolution = m_Texture->GetSize();
	}

}

void dae::Render2DComponent::SetPosition(const float x, const float y)
{
	m_SrcRect.x = x;
	m_SrcRect.y = y;
}

void dae::Render2DComponent::SetResolution(const int width, const int height)
{
	m_TextureResolution.x = width;
	m_TextureResolution.y = height;
	m_SrcRect.z = static_cast<float>(width);
	m_SrcRect.w = static_cast<float>(height);
}

void dae::Render2DComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	auto componentClone = clone->AddComponent<Render2DComponent>(GetComponentTag());
	componentClone->SetTexture(m_TextureFileName);
	componentClone->SetResolution(m_TextureResolution.x, m_TextureResolution.y);
	componentClone->SetDrawStyle(m_DrawStyle);
}

