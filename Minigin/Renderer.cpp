#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

#include "Timers.h"
#include "UpdateContext.h"
#include <assert.h>
#include <algorithm>
#include "Week4Exercises.hpp"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	SDL_GetWindowSize(window, &m_Width, &m_Height);
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}


//void dae::Renderer::DrawImguiDefaultWindow(UpdateContext& context)
//{
//	static std::string frametimeGraphName = "frametime";
//	static ImColor graphColor = m_FrametimeColorTypes.highPerformance;
//
//	if (m_IsFramerateLimitChanged)
//	{
//		m_IsFramerateLimitChanged = false;
//
//		static size_t framerateLimit = static_cast<size_t>(context.GetFrameRateLimit());
//		static const Seconds fpsGraphDuration = 5;
//		size_t newFramerateVecSize = framerateLimit * static_cast<size_t>(fpsGraphDuration);
//		const size_t uncappedFramerateVecSize = 16000;
//
//
//		if (m_FramerateHistory.size() == 0)
//		{
//			m_FramerateHistory.resize(newFramerateVecSize);
//		}
//		else if (framerateLimit == 0)
//		{
//			m_FramerateHistory.resize(uncappedFramerateVecSize);
//		}
//		else if (newFramerateVecSize > m_FramerateHistory.size())
//		{
//			size_t numToAdd = newFramerateVecSize - m_FramerateHistory.size();
//			std::vector<float> newFramerateHistory(newFramerateVecSize, m_FramerateHistory.front());
//			std::copy(m_FramerateHistory.begin() ,m_FramerateHistory.end(), newFramerateHistory.begin() + numToAdd);
//			m_FramerateHistory = newFramerateHistory;
//		}
//		else if (newFramerateVecSize < m_FramerateHistory.size())
//		{
//			size_t numToRemove = m_FramerateHistory.size() - newFramerateVecSize;
//			std::vector<float> newFramerateHistory(newFramerateVecSize);
//			std::copy(m_FramerateHistory.begin() + numToRemove, m_FramerateHistory.end(), newFramerateHistory.begin());
//			m_FramerateHistory = newFramerateHistory;
//		}
//
//	}
//
//	m_FramerateHistory.erase(m_FramerateHistory.begin());
//	m_FramerateHistory.push_back(context.GetDeltaTime() * 1000.f);
//
//
//
//	ImGui::Begin("runtime info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
//	// Draw first plot with multiple sources
//	ImGui::PlotConfig conf;
//	conf.values.count = static_cast<int>(m_FramerateHistory.size());
//	conf.values.ys = m_FramerateHistory.data(); // use ys_list to draw several lines simultaneously
//	conf.values.ys_count = 1;
//	conf.values.color = graphColor;
//	conf.scale.min = 0;
//	conf.scale.max = 1.f / 60.f * 2.f * 1000.f;
//	conf.tooltip.show = true;
//	conf.tooltip.format = "%.0f:%.1fms";
//	conf.grid_x.show = true;
//	conf.grid_x.size = 0;
//	conf.grid_x.subticks = 0;
//	conf.grid_y.size = 1.f / 60.f * 1000.f;
//	conf.grid_y.subticks = 1;
//	conf.grid_y.show = true;
//	conf.frame_size = ImVec2(200, 100);
//	conf.overlay_text = frametimeGraphName.c_str();
//	ImGui::Plot("frametime graph", conf);
//
//
//	ImGui::End();
//}

void dae::Renderer::DrawImguiFramerateWindow(UpdateContext& context)
{
	static float f = 0.0f;
	static int counter = 0;
	static float frameratelimit = context.GetFrameRateLimit();
	static float startTime = dae::PlatformClock::GetTimeInSeconds();

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

	ImGui::Text("Engine Clock Time	: %.3f s", (float)dae::EngineClock::GetTimeInSeconds());
	ImGui::Text("OS Clock Time		: %.3f s", (float)dae::PlatformClock::GetTimeInSeconds() - startTime);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Frame time	: %.3f ms", (float)context.GetDeltaTime().ToMilliseconds());

	ImGui::End();
}

void dae::Renderer::Render(UpdateContext& context)
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	//DrawImguiDefaultWindow(context);
	//SceneManager::GetInstance().RenderUI();

	dae::Ex4::DrawImguiWeek04Exercises();
	SceneManager::GetInstance().RenderUI();

	DrawImguiFramerateWindow(context);
	if (m_showDemo)
		ImGui::ShowDemoWindow(&m_showDemo);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}


void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTextureBackground(const Texture2D& texture) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(0);
	dst.y = static_cast<int>(0);
	dst.w = static_cast<int>(m_Width);
	dst.h = static_cast<int>(m_Height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}


inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
