#include "AnimatedSpriteComponent.h"
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

using namespace engine;

void AnimatedSpriteComponent::Initialize()
{
}

void engine::AnimatedTextureComponent::SetSpritesPerSecond(int spritesPerSecond)
{
	m_SpritesPerSecond = 1.f / spritesPerSecond;
}

void engine::AnimatedTextureComponent::Play(int startSpriteId, int endSpriteId, bool playOnce, bool playBackAndForth)
{
	m_StartSpriteId = startSpriteId;
	m_EndSpriteId = endSpriteId;
	m_PlayOnce = playOnce;
	m_PlayBackAndForth = playBackAndForth;

	m_CurSpriteId = startSpriteId;
	m_AnimationTimer = m_SpritesPerSecond;

	m_Animate = true;
}



void AnimatedSpriteComponent::Update(const engine::UpdateContext& context)
{
	if (m_Animate)
	{
		m_AnimationTimer -= deltaTime;
		if (m_AnimationTimer <= 0)
		{
			if (m_PlayReverse)
			{
				if (--m_CurSpriteId < m_StartSpriteId)
				{
					if (m_PlayBackAndForth)
					{
						m_PlayReverse = !m_PlayReverse;
						m_CurSpriteId = m_StartSpriteId + 1;
					}
					else if (m_PlayOnce)
					{
						m_CurSpriteId = m_StartSpriteId;
						m_Animate = false;
						return;
					}
					else
					{
						m_CurSpriteId = m_EndSpriteId;
					}
				}
			}
			else if (++m_CurSpriteId > m_EndSpriteId)
			{
				if (m_PlayBackAndForth)
				{
					m_PlayReverse = !m_PlayReverse;
					m_CurSpriteId = m_EndSpriteId - 1;
				}
				else if (m_PlayOnce)
				{
					m_CurSpriteId = m_EndSpriteId;
					m_Animate = false;
					return;
				}
				else
				{
					m_CurSpriteId = m_StartSpriteId;
				}
			}

			m_AnimationTimer += m_SpritesPerSecond;
		}
	}
}

void AnimatedSpriteComponent::Draw()
{

		glm::vec2 pos = m_GameObject->m_Transform->GetWorldPosition();
		pos += m_DrawPosition;

		SDL_Rect sourceRect, destinationRect;
		sourceRect.x = m_SrcRect.pos.x;
		sourceRect.y = m_SrcRect.pos.y;
		sourceRect.w = m_SrcRect.size.x;
		sourceRect.h = m_SrcRect.size.y;

		destinationRect.x = static_cast<int>(pos.x);
		destinationRect.y = static_cast<int>(pos.y);
		destinationRect.w = static_cast<int>(pos.y);
		destinationRect.h = static_cast<int>(pos.y);

		//std::cout << "srcX = " << srcX << ", srcY = " << srcY << std::endl;

		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, , m_SpriteHeight, srcX * m_SpriteWidth, srcY * m_SpriteHeight);
}
