#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"
#include "Time.h"

namespace dae {

	class UpdateContext;
	class Seconds;
	class Font;
	class Texture2D;
	class GameTime;
	class TextComponent;
	class FpsComponent final : public BaseComponent
	{
	public:
		FpsComponent(std::shared_ptr<Font> font);
		~FpsComponent();

		virtual void Initialize() override;
		virtual void Update( const UpdateContext& context) override;


		void SetPosition(const float x, const float y);
		void SetColor(int r, int g, int b, int a);
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;

	private:
		TextComponent*					m_TextComponent;
		std::shared_ptr<Font>			m_Font;

		Seconds							m_Accumulator = 0.f;
		uint64_t						m_FpsCount = 0;
		uint64_t						m_FramesPerSecond = 0;
	};
}