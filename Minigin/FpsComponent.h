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
		virtual void Update() override;
		virtual void Draw() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;

		void SetPosition(const float x, const float y);
		void SetColor(int r, int g, int b, int a);
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;

	private:
		std::shared_ptr<TextComponent> m_TextComponent;
		GameTime* m_Time;;
	};
}