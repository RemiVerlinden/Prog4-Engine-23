#include "AnimatedSpriteComponent.h"
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture2D.h"
#include "Common.h"

using namespace engine;
using namespace engine::physics;

void AnimatedSpriteComponent::Initialize()
{
	m_Frametime = 0.2f; // 5 fps
}


void engine::AnimatedSpriteComponent::Update(const engine::UpdateContext& context)
{
	if (m_Pause) return;
	if (!m_Initialized) return;
	if (m_StartSpriteID == m_EndSpriteID) 
	{
		m_CurSpriteID = m_StartSpriteID;
		return;
	} 

	if (m_CyclicTimer.Update(context.GetDeltaTime()))
	{
		if (m_PlayReverse)
		{
			m_CurSpriteID--;
			if (m_CurSpriteID < m_StartSpriteID)
			{
				m_CurSpriteID = m_EndSpriteID;
			}
		}
		else
		{
			m_CurSpriteID++;
			if (m_CurSpriteID > m_EndSpriteID)
			{
				m_CurSpriteID = m_StartSpriteID;
			}
		}
	}


	if (m_PlayOnce)
	{
		if ((m_PlayReverse && m_CurSpriteID == m_StartSpriteID) || (!m_PlayReverse && m_CurSpriteID == m_EndSpriteID))
		{
			m_Pause = true;
			m_PlayOnce = false;
		}
	}
}

void AnimatedSpriteComponent::Draw()
{
	glm::vec2 pos = m_GameObject->m_Transform->GetWorldPosition();

	SDL_RendererFlip flip = static_cast<SDL_RendererFlip>(m_FlipTexture);
	SDL_Rect srcRect = ToSDLRect(m_SrcRect);

	int XspriteID = m_CurSpriteID % m_Cols;
	int YspriteID = m_CurSpriteID / m_Cols;

	srcRect.x = XspriteID * srcRect.w;
	srcRect.y = YspriteID * srcRect.h;

	SDL_Rect dstRect = ToSDLRect(m_DstRect);

	dstRect.x += static_cast<int>(pos.x);
	dstRect.y += static_cast<int>(pos.y);

	dstRect.x *= WindowSettings::scale;
	dstRect.y *= WindowSettings::scale;
	dstRect.w *= WindowSettings::scale;
	dstRect.h *= WindowSettings::scale;

	Renderer::GetInstance().RenderTexture(*m_pTexture, srcRect, dstRect, flip);

}

void engine::AnimatedSpriteComponent::SetTexture(const std::string& filename)
{
	m_TextureFileName = filename;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(m_TextureFileName);

	glm::ivec2 size = m_pTexture->GetSize();
	m_Cols = static_cast<int>(size.x / m_SrcRect.size.x);
	m_Rows = static_cast<int>(size.y / m_SrcRect.size.y);
}



void engine::AnimatedSpriteComponent::SetSourceRect(const Box& srcRect)
{
	m_SrcRect = srcRect;
	if (m_pTexture)
	{
		glm::ivec2 size = m_pTexture->GetSize();
		m_Cols = static_cast<int>(size.x / m_SrcRect.size.x);
		m_Rows = static_cast<int>(size.y / m_SrcRect.size.y);
	}
}

const physics::Box& engine::AnimatedSpriteComponent::GetSourceRect()
{
	return m_SrcRect;
}

void engine::AnimatedSpriteComponent::SetDestinationRect(const Box& dstRect)
{
	m_DstRect = dstRect;
}

const physics::Box& engine::AnimatedSpriteComponent::GetDestinationRect()
{
	return m_DstRect;
}

void engine::AnimatedSpriteComponent::SetFrameTime(float frametime)
{
	m_Frametime = frametime;
	m_CyclicTimer.SetCycleTime(frametime);
}

void engine::AnimatedSpriteComponent::SetFrameRange(int startSpriteID, int endSpriteID)
{
	if (startSpriteID > endSpriteID)
	{
		ENGINE_ERROR("AnimatedSpriteComponent::SetFrameRange > startSpriteID must be smaller than endSpriteID");
		return;
	}
	//int minSpriteID = 0;
	//int maxSpriteID = m_Cols * m_Rows - 1;
	//if (startSpriteID < minSpriteID || maxSpriteID < startSpriteID )
	//{
	//	ENGINE_ERROR("AnimatedSpriteComponent::SetFrameRange > startSpriteID must be between minSpriteID and maxSpriteID");
	//	return;
	//}
	//if (endSpriteID < minSpriteID || maxSpriteID < endSpriteID)
	//{
	//	ENGINE_ERROR("AnimatedSpriteComponent::SetFrameRange > endSpriteID must be between minSpriteID and maxSpriteID");
	//	return;
	//}

	m_Initialized = true;

	m_StartSpriteID = startSpriteID;
	m_CurSpriteID = startSpriteID;
	m_EndSpriteID = endSpriteID;
}
