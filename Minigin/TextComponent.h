#pragma once
#include "BaseComponent.h"
#include <memory>
#include "Transform.h"
#include <SDL_ttf.h>

namespace dae{

	class UpdateContext;
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent( std::shared_ptr<Font> font);
		TextComponent(const std::string& text, std::shared_ptr<Font> font);
		~TextComponent() = default;

		void Initialize() override {};

		virtual void Update(const UpdateContext& context) override;
		virtual void Draw() override;

		void SetText(const std::string& text);
		void SetPosition(const float x, const float y);
		inline glm::vec3 GetPosition();
		void SetColor(int r, int g, int b, int a);

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool						m_needsUpdate;
		std::string					m_text;
		SDL_Color					m_FontColor;
		std::shared_ptr<Font>		m_font;
		std::shared_ptr<Texture2D>	m_textTexture;
	};
}