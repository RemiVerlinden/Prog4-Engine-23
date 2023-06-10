#include "FpsComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <iostream>
#include "TextComponent.h"
#include <format>
#include "GameObject.h"
#include "UpdateContext.h"


void engine::FpsComponent::Initialize()
{
	m_TextComponent = m_GameObject->AddComponent<TextComponent>("SubComponent of FpsComponent: TextComponent");
	m_TextComponent->SetCanBeCloned(false);
	if (m_Font)
		m_TextComponent->SetFont(m_Font);

	m_TextComponent->SetText(std::format("{} FPS", m_FramesPerSecond));
}

void engine::FpsComponent::Update(const UpdateContext& context)
{
	m_Accumulator += context.GetDeltaTime();
	++m_FpsCount;
	if (m_Accumulator >= 1.0f)
	{
		m_FramesPerSecond = m_FpsCount;
		m_FpsCount = 0;
		m_Accumulator -= 1.0f;
		m_TextComponent->SetText(std::format("{} FPS", m_FramesPerSecond));
	}


}

void engine::FpsComponent::DrawUI(UpdateContext& context)
{
	static float f = 0.0f;
	static int counter = 0;
	static float frameratelimit = context.GetFrameRateLimit();
	static float startTime = engine::PlatformClock::GetTimeInSeconds();

	ImGui::Begin("Framerate info");

	ImGui::Spacing();

	if (ImGui::SliderFloat("framerate", &frameratelimit, 0.0f, 750.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
	{
		context.SetFrameRateLimit(frameratelimit);
		//m_IsFramerateLimitChanged = true;
	}


	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();


	ImGui::Spacing();

	ImGui::Text("Engine Clock Time	: %.3f s", (float)engine::EngineClock::GetTimeInSeconds());
	ImGui::Text("OS Clock Time		: %.3f s", (float)engine::PlatformClock::GetTimeInSeconds() - startTime);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Frame time	: %.3f ms", (float)context.GetDeltaTime().ToMilliseconds());

	ImGui::End();
}

void engine::FpsComponent::SetPosition(const float x, const float y)
{
	m_TextComponent->SetPosition(x, y);

}

void engine::FpsComponent::SetColor(int r, int g, int b, int a)
{
	m_TextComponent->SetColor(r, g, b, a);
}

void engine::FpsComponent::SetFont(std::shared_ptr<Font> font)
{
	if (m_TextComponent)
	{
		m_TextComponent->SetFont(font);
		m_Font = font;
	}
	else
	{
		m_Font = font;
	}
}

void engine::FpsComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;

	auto componentClone = clone->AddComponent<FpsComponent>(GetComponentTag());

	glm::vec3 position = m_TextComponent->GetPosition();
	componentClone->SetPosition(position.x, position.y);

	SDL_Color color = m_TextComponent->GetColor();
	componentClone->SetColor(color.r, color.g, color.b, color.a);

	componentClone->SetFont(m_Font);
}

