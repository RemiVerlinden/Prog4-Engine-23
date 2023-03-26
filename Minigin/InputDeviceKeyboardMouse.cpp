#include "InputDeviceKeyboardMouse.h"

//-------------------------------------------------------------------------

namespace dae
{
    namespace Input
    {
        void InputDeviceKeyboardMouse::Initialize()
        {
        }
        void InputDeviceKeyboardMouse::Shutdown()
        {
        }
        bool InputDeviceKeyboardMouse::ProcessInput(Seconds deltaTime)
        {

			m_keyboardState.UpdatePreviousKeyboardState();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				//process event for IMGUI
				ImGui_ImplSDL2_ProcessEvent(&event);

				switch (event.type)
				{
					case SDL_QUIT:
					{
						return false;
						break;
					}
					case SDL_MOUSEWHEEL:
					{
						m_mouseState.ProcesMouseWheel(event.wheel);
					}
						break;
				}
			}

            m_mouseState.ProcessInput(deltaTime);
            m_keyboardState.ProcessInput(deltaTime);

			if (m_keyboardState.WasReleased(SDL_SCANCODE_PAGEUP))
			{
				SceneManager::GetInstance().NextScene();
			}
			if(m_keyboardState.WasReleased(SDL_SCANCODE_PAGEDOWN))
			{
				SceneManager::GetInstance().PreviousScene();
			}


            return true;
        }
    }
}