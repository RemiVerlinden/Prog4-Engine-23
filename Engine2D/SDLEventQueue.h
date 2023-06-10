#pragma once
#include <SDL.h>
#include <deque>
#include "ImguiWrapper.h"

namespace engine
{
	class SDLEventQueue
	{
	public:
		bool PollEvents()
		{
			bool doContinue = true;
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);

				if (event.type == SDL_QUIT)
					doContinue = false;
				
				m_EventQueue.push_back(event);
			}
			return doContinue;
		}
		void EmptyQueue()
		{
			m_EventQueue.clear();
		}

		const std::deque<SDL_Event>& GetEvents() { return m_EventQueue; }

	private:
		std::deque<SDL_Event> m_EventQueue;
	};

}