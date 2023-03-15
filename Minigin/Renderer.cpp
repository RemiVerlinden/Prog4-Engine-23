#include <stdexcept>
#include "UpdateContext.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>
#include <imgui_plot.h>
#include "Time.h"
#include "Minigin.h"

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

static ImVec4 clear_color;
static ImGuiIO io;

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

	io = ImGui::GetIO(); (void)io;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render(UpdateContext& context, TimeStuff& timeThing)
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	{
		static float f = 0.0f;
		static int counter = 0;
		static float frameratelimit = context.GetFrameRateLimit();
		static float startTime = dae::PlatformClock::GetTimeInSeconds();

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &m_showDemo);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &m_showAnotherWindow);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		ImGui::Spacing();

		if (ImGui::SliderFloat("framerate", &frameratelimit, 0.0f, 2000.0f, "%.0f", ImGuiSliderFlags_AlwaysClamp))
		{
			context.SetFrameRateLimit(frameratelimit);
		}

		io.Framerate = 1 / context.GetDeltaTime();

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::Spacing();

		ImGui::Text("Engine Clock Time	: %.3f s", (float)dae::EngineClock::GetTimeInSeconds());
		ImGui::Text("OS Clock Time		: %.3f s", (float)dae::PlatformClock::GetTimeInSeconds() - startTime);
		ImGui::Text("Wanted sleep time	: %.3f ms", (float)timeThing.theoreticalSleepTime);
		ImGui::Text("Actual sleep time	: %.3f ms", (float)timeThing.physicalSleepTime);

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		
		ImGui::Text("Wanted frame time	: %.3f ms", (float)context.GetDeltaTime().ToMilliseconds());
		ImGui::Text("Actual frame time	: %.3f ms", (float)timeThing.physicalFrameTime);

		ImGui::Checkbox("timeToggle", &timeThing.timerToggle);      // Edit bools storing our window open/close state

		ImGui::End();
	}



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
