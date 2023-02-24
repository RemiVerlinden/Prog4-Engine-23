#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"

namespace dae {

	class Font;
	class Texture2D;
	class GameTime;
	class TextComponent;
	class FpsComponent final : public BaseComponent
	{
	public:
		FpsComponent(std::shared_ptr<Font> font);
		~FpsComponent() = default;

		virtual void Initialize(GameTime* time) override;
		virtual void Update(float ts) override;
		virtual void Draw() override;
		virtual void LateUpdate(float ts) override;
		virtual void FixedUpdate(float ts) override;

		void SetPosition(const float x, const float y);


		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;

	private:
		std::shared_ptr<TextComponent> m_TextComponent;
		GameTime* m_Time;;
	};
}