#pragma once
#include "BaseComponent.h"
#include <memory>
#include "IPhysicsSystem.hpp"
#include "Timers.h"

namespace engine
{
	class Texture2D;
	class AnimatedSpriteComponent final : public engine::BaseComponent
	{
	public:
		virtual ~AnimatedSpriteComponent() = default;
		void Initialize() override;

		void Update(const engine::UpdateContext& context) override;
		void Draw() override;

		void SetTexture(const std::string& filename);

		void SetSourceRect(const physics::Box& srcRect);
		const physics::Box& GetSourceRect();
		void SetDestinationRect(const physics::Box& dstRect);
		const physics::Box& GetDestinationRect();
		void SetFrameTime(float frametime);
		float GetFrameTime() { return m_Frametime; }
		void Pause() { m_Pause = true; }
		void Play() { m_Pause = false; }
		bool IsPaused() { return m_Pause; }
		void SetPlayReverse(bool playReverse) { m_PlayReverse = playReverse; }
		void SetFlipTexture(render::FlipTexture flip) { m_FlipTexture = flip; };


		void SetPlayOnce(bool playOnce) { m_PlayOnce = playOnce; }
		void SetFrameRange(int startSpriteID, int endSpriteID);
	private:
		std::shared_ptr<Texture2D>		m_pTexture;
		render::FlipTexture				m_FlipTexture;
		std::string						m_TextureFileName;

		ManualCyclicTimer m_CyclicTimer;

		physics::Box m_SrcRect;
		physics::Box m_DstRect;

		int m_StartSpriteID = 0;
		int m_EndSpriteID = 0;
		int m_CurSpriteID = 0;
		int m_Rows = 0;
		int m_Cols = 0;
		bool m_Pause = false;
		bool m_PlayReverse = false;
		bool m_PlayOnce = false;
		float m_Frametime = 0.0f;

		bool m_Initialized = false;
	};
}